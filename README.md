# G8_Greed_AstraNav-2D_26.2

*Módulo da disciplina*: Greed

## Alunos
| **Matrícula** | **Aluno** |
| --- | --- |
| 251012313 | Luis Gustavo Ferreira Nunes | 
| 251013624 | Mateus Alves Araujo | 

# AstraNav-2D

**AstraNav-2D** é um simulador de planejamento de missão e autonomia de voo para sondas espaciais. O projeto resolve problemas de alocação de carga científica e otimização de rota interplanetária através da aplicação de **Algoritmos Ambiciosos** (*Greedy Algorithms*), integrando a solução a uma interface gráfica 2D em tempo real construída com **Raylib**.

## > --- Algoritmos da Ementa Implementados --- <

### 1. Mochila Fracionária — Alocação de Carga Útil

A sonda possui uma capacidade limite de massa em quilogramas ($W$). Cada módulo ou instrumento científico candidato possui uma massa $w_i$ e um valor de retorno científico $v_i$.

* **Estratégia Ambiciosa:**

  1. Calcula-se a densidade de valor por peso de cada item:

$$
\text{Densidade}_i = \frac{v_i}{w_i}
$$

2. Os itens são ordenados em ordem decrescente de densidade.

3. A carga é preenchida iterativamente com os itens de maior eficiência até que a capacidade máxima $W$ seja atingida.

* **Fração Ótima:** Caso o último item candidato não caiba por inteiro na capacidade restante da sonda, toma-se a fração exata $x_i \in (0, 1)$ necessária para preencher 100% da capacidade sem desperdício de espaço, garantindo a otimalidade global para o modelo fracionário.

### 2. Algoritmo do Caminhoneiro — Reabastecimento Mínimo

A sonda deve percorrer uma rota interplanetária linear contendo postos orbitais de reabastecimento entre a Terra e o destino final, respeitando a autonomia máxima por tanque de combustível ($C$).

* **Estratégia Ambiciosa:** A sonda avança sempre ao longo da trajetória e só efetua uma parada obrigatória no **último posto de reabastecimento alcançável** antes que o nível do tanque de combustível se esgoste.

* **Garantia Matemática:** A escolha ambiciosa assegura o **número mínimo absoluto de paradas** de reabastecimento ao longo da rota interplanetária, minimizando o tempo e o custo logístico da missão.

## > --- Interface Gráfica e Visualização --- <

A aplicação renderiza em tempo real:

* **Mapa do Sistema Solar:** Trajetória pontilhada conectando os corpos celestes e postos orbitais de reabastecimento.

* **Animação da Sonda:** Movimentação visual da nave entre os planetas com indicadores dinâmicos de status de combustível.

* **Painel de Telemetria:**

  * Lista de equipamentos alocados no porão de carga (com indicação clara das frações).

  * Indicador visual nos postos onde a decisão gulosa ativou a parada de reabastecimento.

  * Métrica agregada de retorno científico total gerado.

## > --- Estrutura do Repositório --- <

```
astranav-2d/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── minshell.html
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

## > --- Compilação e Execução --- <

### Pré-requisitos (Modo Nativo)

* Compilador C++20 (`GCC 10+` ou `Clang 11+`)

* `CMake` (v3.16+)

* Biblioteca gráfica `Raylib`

### Passos de Build Nativo

```
# Clone o repositório
git clone https://github.com/projeto-de-algoritmos-2026/G8_Greed_AstraNav-2D_26.2 astranav-2d
cd astranav-2d

# Configure e compile via CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Execute a simulação local
./build/astranav_2d
```

## > --- Build para WebAssembly (Emscripten) --- <

O **AstraNav-2D** pode ser compilado para WebAssembly, permitindo que a simulação corra nativamente e com alto desempenho diretamente em qualquer navegador moderno.

### Pré-requisitos para a Web

* [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html?utm_source=gemini) instalado no sistema.

### Passos para Compilação Web

1. **Ative o ambiente do Emscripten** no seu terminal:

   ```
   source /caminho/para/emsdk/emsdk_env.sh
   ```

2. **Configure o projeto** utilizando a toolchain web e a flag de plataforma:

   ```
   emcmake cmake -B build_web -DCMAKE_BUILD_TYPE=Release -DPLATFORM=Web
   ```

3. **Execute a compilação**:

   ```
   cmake --build build_web
   ```

4. **Teste localmente** (devido às restrições de CORS dos navegadores, utilize um servidor HTTP local):

   ```
   cd build_web
   python3 -m http.server 8080
   ```

   Acesse `http://localhost:8080/astranav_2d.html` no seu navegador.

## > --- | Cronograma de Desenvolvimento (13/09 a 02/10) | --- <

| **Fase** | **Período** | **Objetivos** |
| --- | --- | --- |
| **Fase 1: Módulos Algorítmicos** | 13/09 – 17/09 | Implementação e testes unitários dos algoritmos de Mochila Fracionária e Caminhoneiro em C++. | 
| **Fase 2: Motor Gráfico 2D** | 18/09 – 22/09 | Configuração do Raylib, criação da janela e renderização dos corpos celestes e da sonda. | 
| **Fase 3: Integração e Animação** | 23/09 – 27/09 | Vincular as decisões dos algoritmos à simulação visual e ao painel de telemetria. | 
| **Fase 4: Polimento e Documentação** | 28/09 – 02/10 | Ajustes de UI, validação dos casos de teste e preparação do relatório final. | 

## 📜 Licença

Licença MIT.
