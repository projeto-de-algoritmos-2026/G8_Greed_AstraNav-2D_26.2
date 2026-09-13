# CosmoPath-2D: Planejador de Missão Interplanetária via Algoritmos Gulosos

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Build](https://img.shields.io/badge/build-CMake-green.svg)
![Graphics](https://img.shields.io/badge/graphics-Raylib-red.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

**CosmoPath-2D** é um simulador de planejamento de missão e autonomia de voo para sondas espaciais. O projeto resolve problemas de alocação de carga científica e otimização de rota interplanetária através da aplicação de **Algoritmos Gulosos** (*Greedy Algorithms*), integrando a solução a uma interface gráfica 2D em tempo real.

---

## > Algoritmos da Ementa Implementados 

### Mochila Fracionária — Alocação de Carga Útil
A sonda possui uma capacidade limite de massa em quilogramas ($W$). Cada módulo ou instrumento científico possui uma massa $w_i$ e um valor de retorno científico $v_i$.
- **Estratégia Gulosa:** Ordenação dos itens pela razão de densidade de valor por peso:
  $$\text{Densidade}_i = \frac{v_i}{w_i}$$
- **Resultado:** A carga é preenchida ordenadamente pelos itens de maior densidade. Caso o último item não caiba por inteiro, toma-se a fração exata $x_i \in (0, 1)$ necessária para atingir 100% da capacidade sem desperdício.

### Algoritmo do Caminhoneiro — Reabastecimento Mínimo
A sonda deve percorrer uma rota entre a Terra e um destino distante no Sistema Solar com autonomia máxima por tanque de combustível ($C$).
- **Estratégia Gulosa:** A sonda avança o máximo possível e só efetua uma parada no último posto de reabastecimento alcançável antes que o nível do tanque se esgoste.
- **Resultado:** Garantia matemática do **número mínimo absoluto de paradas** de reabastecimento ao longo da trajetória.

---

## > Interface Gráfica e Visualização

A aplicação renderiza em tempo real:
- **Mapa do Sistema Solar:** Trajetória pontilhada conectando os corpos celestes e postos orbitais de reabastecimento.
- **Animação da Sonda:** Movimentação visual da nave entre os planetas com indicadores de status de combustível.
- **Painel de Telemetria:**
  - Lista de equipamentos alocados no porão de carga (com indicação das frações).
  - Indicador visual nos postos onde a decisão gulosa ativou a parada de reabastecimento.
  - Métrica de retorno científico total gerado.

---


### Planejamento da Estrutura do Repositório

Abaixo está um esboço de como ficará o projeto até o fim do projeto. O intuito é o de orgarnizar e facilitar a identifição de cada camada e pelo que é responsavél.
(pode e deverá sofrer mudanças quando necessario)

```text
cosmopath-2d/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── assets/
│   └── fonts/
├── include/
│   ├── algorithms/
│   │   ├── knapsack.hpp
│   │   └── refueling.hpp
│   ├── core/
│   │   ├── probe.hpp
│   │   └── station.hpp
│   └── render/
│       └── window.hpp
├── src/
│   ├── algorithms/
│   │   ├── knapsack.cpp
│   │   └── refueling.cpp
│   ├── core/
│   │   ├── probe.cpp
│   │   └── station.cpp
│   ├── render/
│   │   └── window.cpp
│   └── main.cpp
└── tests/
    └── test_algorithms.cpp

```

## > Compilação e Execução

### Pré-requisitos
- Compilador C++20 (`GCC 10+` ou `Clang 11+`)
- `CMake` (v3.16+)
- Biblioteca gráfica `Raylib`

### Passos de Build

```bash
# Clone o repositório
git clone https://github.com/seu-usuario/cosmopath-2d.git
cd cosmopath-2d

# Configure e compile via CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Execute a simulação
./build/CosmoPath2D

```
---

## > Cronograma de Desenvolvimento (13/09 a 02/10)

| Fase | Período | Objetivos |
| --- | --- | --- |
| **Fase 1: Módulos Algorítmicos** | 13/09 – 17/09 | Implementação e testes unitários dos algoritmos de Mochila Fracionária e Caminhoneiro em C++. |
| **Fase 2: Motor Gráfico 2D** | 18/09 – 22/09 | Configuração do Raylib, criação da janela e renderização dos corpos celestes e da sonda. |
| **Fase 3: Integração e Animação** | 23/09 – 27/09 | Vincular as decisões dos algoritmos à simulação visual e ao painel de telemetria. |
| **Fase 4: Polimento e Documentação** | 28/09 – 02/10 | Ajustes de UI, validação dos casos de teste e preparação do relatório final. |

---

## 📜 Licença
Licença MIT.
