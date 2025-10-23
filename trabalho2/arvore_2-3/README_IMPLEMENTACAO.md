# Biblioteca de Música - Árvore 2-3

## 📝 Descrição

Implementação completa de uma Biblioteca de Música usando **Árvore 2-3** para organização dos artistas e seus álbuns, conforme especificado na Questão 2 do trabalho de Estruturas de Dados II.

## 🏗️ Estrutura de Dados

### Hierarquia da Organização

```
Árvore 2-3 (Artistas)
    └── Artista
        ├── Nome (chave de busca)
        ├── Estilo musical
        ├── Quantidade de álbuns
        └── Árvore 2-3 (Álbuns)
            └── Álbum
                ├── Título
                ├── Ano de lançamento
                ├── Quantidade de músicas
                └── Lista Ordenada (Músicas)
                    └── Música
                        ├── Título
                        └── Duração (minutos)
```

### Estruturas Utilizadas

1. **Música**: Lista encadeada ordenada por título
2. **Álbum**: Nó de árvore 2-3 contendo lista de músicas
3. **Artista**: Nó de árvore 2-3 principal contendo árvore de álbuns

## ✅ Funcionalidades Implementadas

### 1. Inserção
- ✅ Inserir artista na árvore 2-3 principal
- ✅ Inserir álbum na árvore 2-3 do artista
- ✅ Inserir música na lista ordenada do álbum

### 2. Busca
- ✅ Buscar artista por nome (com caminho percorrido)
- ✅ Buscar álbum dentro de um artista
- ✅ Buscar música dentro de um álbum

### 3. Remoção
- ✅ Remover música de um álbum
- ✅ Remover álbum (com aviso de remoção de todas as músicas)
- ✅ Remover artista (com aviso de remoção de todos álbuns e músicas)

### 4. Visualização
- ✅ Listar artistas em ordem alfabética
- ✅ Mostrar hierarquia da árvore 2-3
- ✅ Exibir informações completas de artista/álbum/música

### 5. Experimento
- ✅ Busca de 30 artistas com análise de desempenho
- ✅ Exibição do caminho percorrido na árvore
- ✅ Medição de tempo de cada busca
- ✅ Estatísticas e análise dos resultados

## 🎯 Requisitos Atendidos

### Da Questão 2:

✅ **Estrutura de dados**:
- Árvore 2-3 para artistas (organizado por nome)
- Árvore 2-3 para álbuns de cada artista
- Lista ordenada para músicas de cada álbum

✅ **Operações obrigatórias**:
- Inserção hierárquica (Artista → Álbum → Música)
- Validação de dependências (música em álbum, álbum em artista)
- Busca em todos os níveis
- Remoção com avisos apropriados

✅ **Experimento de busca**:
- Busca de 30 artistas
- Exibição do caminho na árvore
- Medição de tempo
- Análise dos resultados

## 🔧 Compilação e Execução

### Compilar:
```bash
gcc -o programa arvore_2-3.c -Wall
```

### Executar:
```bash
./programa
```

### No Windows:
```cmd
gcc -o programa.exe arvore_2-3.c
programa.exe
```

## 📊 Menu do Programa

```
1  - Inserir artista
2  - Remover artista
3  - Inserir álbum
4  - Remover álbum
5  - Inserir música
6  - Remover música
7  - Buscar artista
8  - Buscar álbum
9  - Buscar música
10 - Listar artistas (ordem alfabética)
11 - Mostrar hierarquia da árvore
12 - Experimento de busca (30 artistas)
0  - Sair
```

## 🎓 Conceitos Aplicados

### Árvore 2-3
- **Balanceamento automático**: A árvore mantém altura uniforme
- **Nós com 1 ou 2 chaves**: Permite flexibilidade na estrutura
- **Complexidade O(log n)**: Para todas as operações de busca
- **Sem rotações**: Balanceamento por meio de quebras e fusões

### Lista Ordenada
- **Inserção ordenada**: Músicas mantidas em ordem alfabética
- **Complexidade O(n)**: Para inserção e busca linear
- **Estrutura simples**: Adequada para coleções pequenas (músicas de um álbum)

## 📈 Análise do Experimento

O experimento de busca de 30 artistas fornece:

1. **Caminho percorrido**: Mostra os nós visitados em cada nível
2. **Tempo de execução**: Medido em milissegundos para cada busca
3. **Taxa de sucesso**: Percentual de artistas encontrados
4. **Tempo médio**: Permite avaliar o desempenho da estrutura

### Resultados Esperados:
- Tempo médio: < 1 ms para árvores com até 1000 artistas
- Altura da árvore: log₂(n) níveis
- Balanceamento: 100% garantido pela estrutura 2-3

## ⚠️ Observações Importantes

1. **Memória**: A remoção de artista libera toda a memória associada (álbuns e músicas)
2. **Avisos**: O programa alerta o usuário antes de remover álbuns ou artistas
3. **Validação**: Não é possível inserir música sem álbum ou álbum sem artista
4. **Ordenação**: Artistas e álbuns são mantidos em ordem alfabética pela árvore 2-3

## 📝 Arquivos do Projeto

- `arvore_2-3.h` - Declarações de estruturas e funções
- `arvore_2-3.c` - Implementação completa
- `programa` - Executável compilado (Linux)
- `README_IMPLEMENTACAO.md` - Esta documentação

## 👥 Autor

Trabalho desenvolvido para a disciplina de Estruturas de Dados II - UFPI/CSHNB

---

✅ **Status**: Implementação completa e funcional
🎯 **Requisitos**: Todos atendidos conforme especificação da Questão 2
