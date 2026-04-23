&nbsp;
&nbsp;
<p align="center">
  <img width="800" src="./figures/conecta_logo.png" />
</p> 
&nbsp;

# Embedded Language Models for Question and Answer in Industrial Equipment Assistance

### ✍🏾 Authors
[Thommas K. S. Flores](https://github.com/thommaskevin), [João Carlos N. Bittencourt](https://github.com/), [Thiago C. Jesus](https://github.com/), [Daniel G. Costa](https://github.com/daniel-gcosta) and [Ivanovitch Silva](https://github.com/ivanovitchm)

---

This repository provides the **code, datasets, and experimental resources** supporting the paper:

> **Embedded Language Models for Question and Answer in Industrial Equipment Assistance**  
> *Presented at CBA 2026 (XXV Congresso Brasileiro de Automática)*

The study proposes an **end-to-end pipeline for offline technical question answering** on microcontroller-class hardware, targeting **Industry 4.0** scenarios where connectivity is restricted. It integrates synthetic question–answer generation from industrial manuals, training of a **decoder‑only Extreme Small Language Model (Extreme SLM)**, conversion to **self‑contained C++ code**, and deployment on an **ESP32** platform with **Bluetooth** interaction—all without cloud dependence.

---

## 📁 Repository Structure

```plaintext
Extreme-TinyGPT-Industrial-QA/
├── src/
│   ├── code/
│   │   ├── arduino/                 # ESP32 inference firmware (C++)
│   │   └── model_export/            # Generated C++ model artifacts
│   ├── data/
│   │   ├── raw/                     # Original technical manuals (PDF)
│   │   ├── processed/               # Chunked and cleaned text segments
│   │   └── synthetic_qa/            # Generated Q&A pairs (JSONL)
│   ├── notebooks/
│   │   ├── 01_manual_chunking.ipynb
│   │   ├── 02_synthetic_qa_generation.ipynb
│   │   ├── 03_llm_as_judge_filtering.ipynb
│   │   └── 04_training_analysis.ipynb
│   ├── python/
│   │   ├── extreme_slm/
│   │   │   ├── model.py             # Decoder‑only Transformer
│   │   │   ├── trainer.py
│   │   │   ├── tokenizer.py
│   │   │   └── export_to_cpp.py     # PyTorch → C++ code generator
│   │   └── pipeline/
│   │       ├── pdf_extractor.py
│   │       ├── chunker.py
│   │       ├── qa_generator.py      # Qwen3‑Coder via Ollama
│   │       └── verifier.py          # LLM‑as‑a‑judge module
│   ├── figures/
│   │   ├── conecta_logo.png
│   │   ├── architecture_diagram.pdf
│   │   ├── parallel_coordinates.pdf
│   │   ├── pareto_frontier.pdf
│   │   └── bluetooth_session.png
│   └── results/                     # Hyperparameter logs and metrics
├── .gitignore
├── LICENSE
├── README.md
└── requirements.txt
```

---

## 📤 Installation

* **Python 3.9+** is recommended.
* **Ollama** must be installed locally to run Qwen3‑Coder for dataset generation.
* **Arduino IDE** or **PlatformIO** is required for ESP32 deployment.

```bash
git clone https://github.com/<your-org>/Extreme-TinyGPT-Industrial-QA.git
cd Extreme-TinyGPT-Industrial-QA
python -m venv venv
source venv/bin/activate      # Linux/macOS
venv\Scripts\activate         # Windows
pip install -r requirements.txt
```

## 📚 Jupyter Notebooks

-  [![Python](https://img.shields.io/badge/-Notebook-191A1B?style=flat-square&logo=jupyter)]( )  Exploratory analysis of battery and driving data

-  [![Python](https://img.shields.io/badge/-Notebook-191A1B?style=flat-square&logo=jupyter)]( )  Training, physics-informed loss configuration, and quantization

-  [![Python](https://img.shields.io/badge/-Notebook-191A1B?style=flat-square&logo=jupyter)]( )  Comparative evaluation of prediction accuracy, inference time, memory usage, and energy consumption


## 📚 Arduino Code

-  [![Jupyter](https://img.shields.io/badge/Arduino-00878F?logo=arduino&logoColor=fff&style=plastic)]( ) Extended Kalman Filter (EKF)

---

## 📚 Dataset

The corpus is derived from the technical manual of a **smart pressure valve**. The pipeline:

1. **Extracts** text from PDF pages.  
2. **Chunks** content using structure‑aware boundaries (headings, chapters).  
3. **Filters** low‑value segments (headers, footers, page numbers).  
4. **Generates** synthetic Q&A pairs using **Qwen3‑Coder** (served via Ollama).  
5. **Verifies** each pair with an **LLM‑as‑a‑judge** step (labels: `high`, `low`, `rejected`).

The final dataset contains **verified Q&A pairs** in JSONL format, including source metadata and confidence labels.

---

## 🧠 Extreme SLM Architecture

The **Extreme SLM** is a **decoder‑only Transformer** with masked causal self‑attention, tailored for extreme resource constraints.

| Component            | Details                                      |
|----------------------|----------------------------------------------|
| Embedding dimension  | 12 or 24                                     |
| Attention heads      | 2 or 4                                       |
| Transformer layers   | 1 or 2                                       |
| Activation           | GeLU                                         |
| Context window       | 23 tokens                                    |
| Vocabulary size      | 1,707 tokens                                 |
| Framework            | PyTorch → C++ code generation                |

The model is trained with **cross‑entropy loss** on the synthetic Q&A pairs. After training, a custom exporter translates the weights into **static C++ arrays** and an **autoregressive inference loop** ready for embedded deployment.

---

## 🔧 Training & Hyperparameter Search

A grid search over **128 configurations** was performed, varying:

- Embedding dimension
- Number of heads
- Number of layers
- Dropout rate
- Learning rate
- Batch size
- Training epochs

**Key Finding:** Embedding dimension had the strongest influence on generation quality; depth and heads showed marginal effects at this scale.

---

## 🚀 On‑Device Deployment

The best Extreme SLM configuration was deployed on an **ESP32‑WROOM‑32 DevKit V1**:

| Metric                     | Value                 |
|----------------------------|-----------------------|
| **Flash usage**            | 1.20 MB              |
| **RAM usage (global)**     | 56.1 KB              |
| **Power consumption**      | 754.1 mW             |
| **Inference latency**      | 318 ms               |
| **Interaction**            | Bluetooth Serial      |

The firmware receives queries from an Android smartphone via Bluetooth, performs **autoregressive token generation**, and returns the answer **completely offline**.

<p align="center">
  <img width="600" alt="Bluetooth session showing Q&A on ESP32" src="./figures/bluetooth_session.png">
</p>

---

## 📊 Results Overview

- **Lowest test loss:** 6.933 (embedding=24, heads=4, layers=2)  
- **Best BLEU:** 0.156  
- **Exact match (non‑zero):** achieved in 14% of configurations  
- **Pareto‑optimal models:** 193–220 KB for ~8.5 test loss  

The **interactive response time** (including Bluetooth communication) was **~860 ms**, with **318 ms** of pure on‑device inference.

---

## 🧠 Conclusion

This work demonstrates a **complete, hardware‑aware pipeline** for embedding language‑model‑based technical assistance directly into **microcontroller‑class industrial devices**.  

The approach advances **offline, privacy‑preserving intelligence at the edge**, reducing reliance on cloud connectivity and specialized personnel. The main limitations—dataset size and context window—point to future work in **quantization, pruning, and hardware‑software co‑design**.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE) – © 2026 Conect2ai.

---

## 📖 Citation

If you use this work in your research, please cite:

```bibtex
@inproceedings{flores2026embedded,
  title     = {Embedded Language Models for Question and Answer in Industrial Equipment Assistance},
  author    = {Flores, Thommas K. S. and Bittencourt, Jo{\~a}o Carlos N. and Jesus, Thiago C. and Costa, Daniel G. and Silva, Ivanovitch},
  booktitle = {Proceedings of the XXV Congresso Brasileiro de Autom{\'a}tica (CBA 2026)},
  year      = {2026},
  note      = {To appear}
}
```
```
---

## 📄 License

This package is licensed under the [MIT License](https://github.com/conect2ai/Conect2Py-Package/blob/main/LICENSE) - © 2023 Conect2ai.

