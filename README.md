# Krishi Sahayak — ML / TinyML Pipeline
### Smart India Hackathon 2026 | Hardware Category
### ESP32-S3 Edge AI Crop Disease Detection

---

## Overview

This folder contains the complete machine learning pipeline for the **Krishi Sahayak Smart Farming Assistant Node** — an ESP32-S3-based field device that performs fully offline crop disease detection using a camera and onboard AI inference.

The ML pipeline takes a raw camera frame, classifies the wheat leaf into one of two classes, and outputs a structured result into the firmware's shared payload:

```
[Disease_ID | Severity_% | VWC_% | VPD_kPa | Water_Level_Stage | Valve_Close_Flag | Risk_Flags]
                ↑ this pipeline owns these two fields
```

**No cloud dependency at inference time. Everything runs on-device.**

---

## Scope (Demo — SIH 2026)

| Item | Decision |
|---|---|
| Crop | Wheat |
| Classes | `0 = Healthy`, `1 = Leaf_Rust` |
| Nutrient deficiency | Deferred — no public wheat-specific dataset exists; planned for post-demo self-collection under the multispectral rig |
| Pest detection | Deferred to future scope |
| Dataset | `jocelyndumlao/wheat-nitrogen-deficiency-and-leaf-rust-image` (Kaggle) — 859 images total |
| Camera | OV7670 (VGA 640×480, DVP parallel interface) |
| Input tensor | 224×224×3, float32 normalized to [0,1] |
| Inference framework | esp-dl (Espressif), NOT generic TFLite Micro |

---

## Results

| Metric | Value |
|---|---|
| Float32 val accuracy | 92% |
| INT8 (QAT) val accuracy | 88% |
| Accuracy drop after quantization | 3.6% |
| Leaf_Rust recall | 0.80 |
| Leaf_Rust precision | 0.92 |
| Final .espdl model size | 2613 KB |
| Inference budget (target) | < 300 ms end-to-end |

Leaf_Rust recall of 0.80 means the device correctly flags 4 out of 5 diseased leaves. False negative rate (missed disease) is more costly than false positives in this domain — this was accounted for via class weighting during training (`Leaf_Rust` weighted 1.5x).

---

## Pipeline Architecture

```
Dataset (Kaggle)
      │
      ▼
Phase 1 — Data Pipeline
  • Pool all pre-existing splits (train/val/test)
  • Re-split 80/20 (seed=42) via splitfolders
  • Resize to 224×224×3
  • Normalize to [0, 1]
  • ExG leaf masking DISABLED — rust lesions are orange/brown,
    ExG masks them out, destroying the most critical diagnostic features
      │
      ▼
Phase 2 — MobileNetV2 Transfer Learning
  • Base: MobileNetV2 (ImageNet pretrained, include_top=False)
  • Custom head: GlobalAveragePool → Dense(128, ReLU) → Dropout(0.3) → Dense(2, Softmax)
  • Phase A: Frozen base, LR=1e-3, 10 epochs (head training only)
  • Phase B: Unfreeze last 10 layers, LR=1e-5, 20 epochs (fine-tuning)
  • Class weight: {Healthy: 1.0, Leaf_Rust: 1.5}
  • Result: 92% val accuracy
      │
      ▼
Phase 3 — Quantization-Aware Training (QAT) + INT8 Conversion
  • QAT via tensorflow-model-optimization (TF_USE_LEGACY_KERAS=1 required)
  • QAT fine-tune: LR=1e-5, 10 epochs on same augmented pipeline
  • Post-QAT float accuracy: 89.6%
  • Convert QAT model → TFLite INT8 (float32 I/O, full calibration set)
  • INT8 val accuracy: 88% (drop: 3.6%) ✅
      │
      ▼
Phase 4A — ONNX Export + ESP-PPQ Quantization (local, Python 3.11)
  • Export Keras float model → ONNX (opset 13) via tf2onnx
  • Simplify ONNX graph via onnx-simplifier (removes redundant Transpose/Reshape nodes)
  • Quantize to .espdl format via ESP-PPQ (KL-divergence calibration, esp32s3 target)
  • Output: wheat_model.espdl (2613 KB) — the file esp-dl loads on-device
      │
      ▼
Phase 4B — ESP-IDF Firmware Integration (C/C++)
  [in firmware/ folder — owned by firmware teammate]
```

---

## Key Engineering Decisions

**Why ExG masking was disabled:**
ExG (Excess Green Index) highlights green pixels and suppresses background. Rust-infected regions are orange/brown — ExG treats them as background and zeroes them out. Since rust lesions are the primary diagnostic feature, masking actively destroys the signal we need. Simple resize-only preprocessing was used instead.

**Why QAT instead of Post-Training Quantization (PTQ):**
Standard PTQ produced an unacceptable accuracy drop (9–18%) across multiple calibration configurations. QAT trains the model knowing it will be quantized, resulting in weights that tolerate INT8 rounding — bringing the drop down to 3.6%.

**Why esp-dl requires .espdl, not .tflite:**
esp-dl is Espressif's proprietary inference library optimized for the LX7 dual-core architecture. It does not load TFLite files. The conversion chain must go through ESP-PPQ (Espressif's quantization tool) to produce the .espdl format. ESP-PPQ only accepts ONNX input, hence the Keras → ONNX → .espdl chain.

**Why Python 3.11 was required locally:**
ESP-PPQ caps at Python <3.13. Google Colab's current default is Python 3.13. ESP-PPQ was run locally in a dedicated Python 3.11 virtual environment on Windows.

**Why nitrogen deficiency was deferred:**
No clean, publicly available wheat-specific nitrogen deficiency leaf image dataset exists on Kaggle or other public sources. The `jocelyndumlao` dataset, despite its name, contains only Healthy/Leaf_Rust classes. The `raiaone/early-nutrient-stress-detection-of-plants` dataset contains only gourd crops (ashgourd, bittergourd, snakegourd) — unusable for wheat. Self-collection under the multispectral rig is the planned path for post-demo Phase 2.

---

## File Structure

```
ml/
├── phase1_data_pipeline.py        # Colab: dataset download, organization, preprocessing
├── phase2_training.py             # Colab: MobileNetV2 transfer learning + fine-tuning
├── phase3_quantization.py         # Colab: QAT + TFLite INT8 conversion
├── phase4a_step1_onnx_export.py   # Colab: Keras → ONNX export + operator inventory
├── phase4a_step2_espdl_quantize.py# Colab: ESP-PPQ setup (reference — run locally)
├── quantize_for_espdl.py          # LOCAL (Python 3.11): ONNX → .espdl via ESP-PPQ
└── models/
    ├── wheat_model.espdl          # Final quantized model for esp-dl (firmware input)
    └── wheat_int8.tflite          # INT8 TFLite (validation reference only, not used on-device)
```

---

## How to Reproduce

### Training (Google Colab, T4 GPU runtime)

```python
# Required at the very start of the Colab session — before any TF import
import os
os.environ['TF_USE_LEGACY_KERAS'] = '1'
```

1. Run `phase1_data_pipeline.py` cells 1–8 (needs Kaggle API token)
2. Run `phase2_training.py` cells 9–17
3. Run `phase3_quantization.py` QAT cells (18–22)
4. Run `phase4a_step1_onnx_export.py` cells A1–A5
5. Download `wheat_model_simplified.onnx` and train images from Colab

### ESP-PPQ Quantization (Windows local, Python 3.11)

```cmd
# Install Python 3.11 from python.org, then:
py -3.11 -m venv C:\espdl_env
C:\espdl_env\Scripts\activate
pip install torch torchvision --index-url https://download.pytorch.org/whl/cpu
pip install esp-ppq onnx onnx-simplifier opencv-python

# Update paths in script, then:
python quantize_for_espdl.py
```

### Dependencies

| Tool | Version / Notes |
|---|---|
| TensorFlow | 2.16+ (Colab default) |
| tensorflow-model-optimization | Latest compatible with TF_USE_LEGACY_KERAS=1 |
| tf2onnx | Latest |
| onnx + onnx-simplifier | Latest |
| esp-ppq | Latest (requires Python 3.8–3.12) |
| PyTorch | CPU-only build sufficient for quantization |
| splitfolders | Latest |
| OpenCV | opencv-python |

---

## Class Index Mapping

This mapping is fixed and must match exactly in both training code and firmware:

```
0 = Healthy
1 = Leaf_Rust
```

The firmware reads `Disease_ID` (0 or 1) from the model output argmax and maps it to the advisory text shown to the farmer.

---

## Future Scope (Post-Demo)

- **Nitrogen deficiency class:** Self-collect 150+ wheat leaf images under the OV7670 + multispectral LED rig, add as class 2, retrain
- **Severity estimation:** Current `Severity_%` is rule-based (confidence score × 100); a segmentation head could give pixel-level lesion coverage as a more accurate severity metric
- **Additional diseases:** Yellow rust, powdery mildew — datasets exist but were not merged to avoid domain-shift issues in the demo model
- **On-device latency benchmark:** Real <300ms verification to be done in Phase 4B using esp-dl's built-in profiler on actual ESP32-S3 hardware

---

*ML pipeline by Vardan | SIH 2026 Team — Krishi Sahayak*
