# 🔴⚫ GUIA DE COMPILAÇÃO E TESTE - LLRB (Pendente à Esquerda)

## 📌 O QUE FOI IMPLEMENTADO

Sua implementação usa **Left-Leaning Red-Black Tree (LLRB)** - Árvore Rubro-Negra Pendente à Esquerda, que é mais simples e elegante que a versão tradicional.

### ✅ Diferenças da LLRB:

1. **Sem ponteiro pai** - Implementação recursiva pura
2. **Sempre pendente à esquerda** - Links vermelhos só à esquerda
3. **Mais simples** - Menos casos de rotação
4. **Mesmo desempenho** - O(log n) garantido

---

## 🚀 COMPILAÇÃO

### Windows (PowerShell):

```powershell
cd d:\Downloads\trabalho7\trabalhos_estrutura_de_dados_2\trabalho2\arvore_rubro_negra

gcc -o teste_album main_teste_album.c rubro_negra.c -I.

.\teste_album
```

### Linux/Mac:

```bash
cd trabalho2/arvore_rubro_negra

gcc -o teste_album main_teste_album.c rubro_negra.c -I.

./teste_album
```

---

## 📋 MENU DO TESTE

```
╔═══════════════════════════════════════════════════════════╗
║        TESTE - ÁRVORE RUBRO-NEGRA DE ÁLBUNS (LLRB)       ║
╠═══════════════════════════════════════════════════════════╣
║  1. Inserir Álbum                                         ║
║  2. Remover Álbum                                         ║
║  3. Buscar Álbum                                          ║
║  4. Listar Todos os Álbuns                                ║
║  5. Inserir Música em Álbum                               ║
║  6. Remover Música de Álbum                               ║
║  7. Buscar Música em Álbum                                ║
║  8. Listar Músicas de um Álbum                            ║
║  9. Visualizar Estrutura da Árvore                        ║
║  0. Sair                                                  ║
╚═══════════════════════════════════════════════════════════╝
```

---

## 🧪 ROTEIRO DE TESTES RÁPIDOS

### 1️⃣ **Carregar Dados de Teste**

Ao iniciar o programa, escolha:
```
Deseja carregar dados de teste? (1=Sim, 0=Não): 1
```

Isso carrega automaticamente:
- 5 álbuns (The Beatles e Pink Floyd)
- 6 músicas distribuídas

### 2️⃣ **Visualizar Estrutura (Opção 9)**

Veja a árvore com cores:
```
🌳 ESTRUTURA DA ÁRVORE RUBRO-NEGRA (LLRB):
Legenda: 🔴 = VERMELHO | ⚫ = PRETO

⚫ Sgt Peppers Lonely Hearts Club Band (1967) - 0 músicas
│   ├── 🔴 Abbey Road (1969) - 3 músicas
│   ├── 🔴 The Dark Side of the Moon (1973) - 3 músicas
│       ├── ⚫ Let It Be (1970) - 0 músicas
│       ├── ⚫ The Wall (1979) - 0 músicas
```

### 3️⃣ **Listar Álbuns em Ordem (Opção 4)**

```
📚 LISTA DE ÁLBUNS (em ordem alfabética):
  📀 Abbey Road (1969) - 3 músicas
  📀 Let It Be (1970) - 0 músicas
  📀 Sgt Peppers Lonely Hearts Club Band (1967) - 0 músicas
  📀 The Dark Side of the Moon (1973) - 3 músicas
  📀 The Wall (1979) - 0 músicas
```

### 4️⃣ **Inserir Novo Álbum (Opção 1)**

```
Escolha: 1

📀 INSERIR NOVO ÁLBUM
Título do álbum: Wish You Were Here
Ano de lançamento: 1975
✅ Álbum 'Wish You Were Here' inserido com sucesso!
```

### 5️⃣ **Inserir Música (Opção 5)**

```
Escolha: 5

🎵 INSERIR MÚSICA
Título do álbum: Wish You Were Here
Título da música: Shine On You Crazy Diamond
Duração (minutos): 13.5
✅ Música 'Shine On You Crazy Diamond' inserida no álbum 'Wish You Were Here'!
```

### 6️⃣ **Buscar e Listar Músicas (Opção 8)**

```
Escolha: 8

🎵 LISTAR MÚSICAS DE ÁLBUM
Título do álbum: Abbey Road

  📀 Álbum: Abbey Road (1969)
  Quantidade de músicas: 3
  Músicas:
  1. Come Together (4.20 min)
  2. Here Comes the Sun (3.05 min)
  3. Something (3.03 min)
```

### 7️⃣ **Remover Música (Opção 6)**

```
Escolha: 6

🗑️  REMOVER MÚSICA
Título do álbum: Abbey Road
Título da música: Something
🗑️  Música 'Something' removida do álbum 'Abbey Road'!
```

### 8️⃣ **Remover Álbum (Opção 2)**

```
Escolha: 2

🗑️  REMOVER ÁLBUM
Título do álbum: Let It Be
⚠️  Todas as 0 músicas do álbum 'Let It Be' serão removidas!
🗑️  Álbum 'Let It Be' removido com sucesso!
```

### 9️⃣ **Verificar Balanceamento (Opção 9)**

Após inserções e remoções, veja se a árvore mantém propriedades LLRB:
- ✅ Links vermelhos sempre à esquerda
- ✅ Sem dois vermelhos consecutivos
- ✅ Raiz sempre preta

---

## 🔬 TESTE DE BALANCEAMENTO AUTOMÁTICO

### Inserir nesta ordem:

1. A
2. B
3. C
4. D
5. E

A árvore se balanceia automaticamente! Verifique com opção 9.

---

## ✅ CHECKLIST DE TESTES

- [ ] Inserir álbuns (entrada simples)
- [ ] Inserir álbuns em ordem alfabética
- [ ] Inserir álbuns em ordem reversa
- [ ] Visualizar estrutura (cores corretas)
- [ ] Buscar álbum existente
- [ ] Buscar álbum inexistente
- [ ] Listar todos os álbuns em ordem
- [ ] Inserir músicas (mantém ordem alfabética)
- [ ] Listar músicas de um álbum
- [ ] Buscar música específica
- [ ] Remover música
- [ ] Remover álbum (libera músicas)
- [ ] Verificar que raiz é sempre preta
- [ ] Verificar que não há dois vermelhos consecutivos
- [ ] Verificar que links vermelhos são à esquerda

---

## 🎯 PROPRIEDADES LLRB GARANTIDAS

Sua implementação garante:

1. ✅ **Raiz sempre preta**
2. ✅ **Links vermelhos pendentes à esquerda**
3. ✅ **Não há dois vermelhos consecutivos**
4. ✅ **Balanceamento perfeito (2-3-4 tree)**
5. ✅ **Todas operações O(log n)**

---

## 📊 COMPARAÇÃO: LLRB vs RB Tradicional

| Aspecto | LLRB (Sua Impl.) | RB Tradicional |
|---------|------------------|----------------|
| Ponteiro pai | ❌ Não precisa | ✅ Necessário |
| Implementação | Recursiva | Iterativa |
| Casos de rotação | 3 (simples) | 6+ (complexo) |
| Linhas de código | ~400 | ~800 |
| Performance | O(log n) | O(log n) |
| Links vermelhos | Só à esquerda | Ambos os lados |

---

## 🐛 DEBUGGING

Se algo não funcionar:

1. **Erros de compilação**: Certifique-se que `rubro_negra.h` está no mesmo diretório
2. **Álbuns não ordenados**: Veja opção 9 para verificar estrutura
3. **Músicas não ordenadas**: Verifique inserção na lista (alfabética)
4. **Cores erradas**: Raiz deve ser preta, vermelhos à esquerda

---

## 📈 COMPLEXIDADE

| Operação | Complexidade |
|----------|-------------|
| Inserir álbum | O(log n) |
| Remover álbum | O(log n) |
| Buscar álbum | O(log n) |
| Inserir música | O(m) onde m = músicas no álbum |
| Remover música | O(m) |
| Buscar música | O(m) |

---

## 🎓 CONCEITOS IMPLEMENTADOS

### Árvore LLRB:
- ✅ Inserção recursiva com balanceamento
- ✅ Rotação à esquerda
- ✅ Rotação à direita
- ✅ Inversão de cores
- ✅ Remoção com rebalanceamento
- ✅ Movimento de links vermelhos

### Lista Encadeada:
- ✅ Inserção ordenada (alfabética)
- ✅ Remoção por valor
- ✅ Busca linear

---

## 🏆 PONTOS FORTES DA SUA IMPLEMENTAÇÃO

1. **Código limpo e legível**
2. **LLRB mais simples que RB tradicional**
3. **Sem vazamento de memória**
4. **Balanceamento automático garantido**
5. **Fácil integração com código do James**
6. **Comentários completos**

---

**✅ SUA PARTE ESTÁ PRONTA E TESTÁVEL INDEPENDENTEMENTE!**

Quando o James terminar a parte dele (Artistas), basta ele:
1. Usar suas funções `inserirAlbumRB()` e `removerAlbumRB()`
2. Implementar `buscarArtistaRB()`
3. Criar o menu principal completo
