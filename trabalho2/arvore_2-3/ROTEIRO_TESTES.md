# 🧪 ROTEIRO DE TESTES - BIBLIOTECA DE MÚSICA (ÁRVORE 2-3)

## 📋 Objetivo
Validar todas as funcionalidades da implementação da Árvore 2-3 conforme especificado na Questão 2 do trabalho.

---

## 🚀 TESTE 1: Inserção de Artistas

### Objetivo
Testar a inserção de artistas e verificar o balanceamento da árvore 2-3.

### Passos:
1. Executar o programa: `./programa`
2. Selecionar opção **1** (Inserir artista)
3. Inserir os seguintes artistas:

```
Artista 1:
Nome: The Beatles
Estilo: Rock

Artista 2:
Nome: Pink Floyd
Estilo: Rock Progressivo

Artista 3:
Nome: Queen
Estilo: Rock

Artista 4:
Nome: Led Zeppelin
Estilo: Hard Rock

Artista 5:
Nome: Bob Dylan
Estilo: Folk Rock

Artista 6:
Nome: AC/DC
Estilo: Hard Rock

Artista 7:
Nome: Metallica
Estilo: Heavy Metal
```

### Verificação:
4. Selecionar opção **10** (Listar artistas em ordem alfabética)
5. **Resultado esperado**: Artistas aparecem ordenados:
   - AC/DC
   - Bob Dylan
   - Led Zeppelin
   - Metallica
   - Pink Floyd
   - Queen
   - The Beatles

6. Selecionar opção **11** (Mostrar hierarquia da árvore)
7. **Resultado esperado**: Ver a estrutura balanceada da árvore 2-3

---

## 🎸 TESTE 2: Inserção de Álbuns

### Objetivo
Testar a inserção de álbuns dentro dos artistas.

### Passos:
1. Selecionar opção **3** (Inserir álbum)
2. Inserir os seguintes álbuns:

```
Para The Beatles:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Nome do artista: The Beatles
Título do álbum: Abbey Road
Ano de lançamento: 1969

Nome do artista: The Beatles
Título do álbum: Sgt. Pepper's Lonely Hearts Club Band
Ano de lançamento: 1967

Nome do artista: The Beatles
Título do álbum: Let It Be
Ano de lançamento: 1970

Para Pink Floyd:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Nome do artista: Pink Floyd
Título do álbum: The Dark Side of the Moon
Ano de lançamento: 1973

Nome do artista: Pink Floyd
Título do álbum: The Wall
Ano de lançamento: 1979

Para Queen:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Nome do artista: Queen
Título do álbum: A Night at the Opera
Ano de lançamento: 1975

Nome do artista: Queen
Título do álbum: News of the World
Ano de lançamento: 1977
```

### Verificação:
3. Selecionar opção **7** (Buscar artista)
4. Buscar "The Beatles"
5. **Resultado esperado**: 
   ```
   🎤 Artista: The Beatles
   Estilo: Rock
   Quantidade de álbuns: 3
   ```

---

## 🎵 TESTE 3: Inserção de Músicas

### Objetivo
Testar a inserção de músicas em álbuns e verificar ordenação.

### Passos:
1. Selecionar opção **5** (Inserir música)
2. Inserir as seguintes músicas:

```
Para Abbey Road (The Beatles):
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Nome do artista: The Beatles
Título do álbum: Abbey Road
Título da música: Come Together
Duração: 4.20

Nome do artista: The Beatles
Título do álbum: Abbey Road
Título da música: Something
Duração: 3.03

Nome do artista: The Beatles
Título do álbum: Abbey Road
Título da música: Here Comes the Sun
Duração: 3.05

Para The Dark Side of the Moon (Pink Floyd):
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Nome do artista: Pink Floyd
Título do álbum: The Dark Side of the Moon
Título da música: Time
Duração: 6.53

Nome do artista: Pink Floyd
Título do álbum: The Dark Side of the Moon
Título da música: Money
Duração: 6.23

Nome do artista: Pink Floyd
Título do álbum: The Dark Side of the Moon
Título da música: Us and Them
Duração: 7.49

Para A Night at the Opera (Queen):
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Nome do artista: Queen
Título do álbum: A Night at the Opera
Título da música: Bohemian Rhapsody
Duração: 5.55

Nome do artista: Queen
Título do álbum: A Night at the Opera
Título da música: Love of My Life
Duração: 3.38
```

### Verificação:
3. Selecionar opção **8** (Buscar álbum)
4. Buscar:
   ```
   Nome do artista: The Beatles
   Título do álbum: Abbey Road
   ```
5. **Resultado esperado**:
   ```
   📀 Álbum: Abbey Road (1969)
   Quantidade de músicas: 3
   Músicas:
     1. Come Together (4.20 min)
     2. Here Comes the Sun (3.05 min)
     3. Something (3.03 min)
   ```
   **Observar que as músicas estão em ordem alfabética!**

---

## 🔍 TESTE 4: Busca de Músicas

### Objetivo
Testar a busca de músicas específicas.

### Passos:
1. Selecionar opção **9** (Buscar música)
2. Testar as seguintes buscas:

```
Busca 1 (Sucesso):
━━━━━━━━━━━━━━━━━━━━━━━━━━
Nome do artista: Queen
Título do álbum: A Night at the Opera
Título da música: Bohemian Rhapsody

Resultado esperado:
🎵 Música encontrada!
  Título: Bohemian Rhapsody
  Duração: 5.55 minutos

Busca 2 (Falha - música não existe):
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Nome do artista: The Beatles
Título do álbum: Abbey Road
Título da música: Yesterday

Resultado esperado:
❌ Música não encontrada!

Busca 3 (Falha - álbum não existe):
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Nome do artista: The Beatles
Título do álbum: Rubber Soul
Título da música: Norwegian Wood

Resultado esperado:
❌ Álbum 'Rubber Soul' não encontrado para o artista 'The Beatles'!
```

---

## 🗑️ TESTE 5: Remoção de Música

### Objetivo
Testar a remoção de músicas individuais.

### Passos:
1. Selecionar opção **6** (Remover música)
2. Remover:
   ```
   Nome do artista: The Beatles
   Título do álbum: Abbey Road
   Título da música: Something
   ```
3. **Resultado esperado**:
   ```
   🗑️  Música 'Something' removida do álbum 'Abbey Road'!
   ```

### Verificação:
4. Selecionar opção **8** (Buscar álbum)
5. Buscar "Abbey Road" de "The Beatles"
6. **Resultado esperado**: Apenas 2 músicas (Something foi removida)

---

## 🗑️ TESTE 6: Remoção de Álbum

### Objetivo
Testar a remoção de álbuns com aviso ao usuário.

### Passos:
1. Selecionar opção **4** (Remover álbum)
2. Remover:
   ```
   Nome do artista: Pink Floyd
   Título do álbum: The Wall
   ```
3. **Resultado esperado**:
   ```
   ⚠️  Todas as músicas do álbum 'The Wall' serão removidas!
   🗑️  Álbum 'The Wall' e todas suas músicas foram removidos!
   ```

### Verificação:
4. Tentar buscar o álbum "The Wall"
5. **Resultado esperado**: Álbum não encontrado

---

## 🗑️ TESTE 7: Remoção de Artista

### Objetivo
Testar a remoção de artista com todos os seus dados.

### Passos:
1. Selecionar opção **2** (Remover artista)
2. Remover:
   ```
   Nome do artista: Led Zeppelin
   ```
3. **Resultado esperado**:
   ```
   ⚠️  Todos os X álbuns e músicas do artista 'Led Zeppelin' serão removidos!
   🗑️  Artista 'Led Zeppelin' removido com sucesso!
   ```

### Verificação:
4. Selecionar opção **10** (Listar artistas)
5. **Resultado esperado**: Led Zeppelin não aparece na lista

---

## 📊 TESTE 8: Experimento de Busca (30 Artistas)

### Objetivo
Executar o experimento principal do trabalho.

### Preparação:
1. Inserir 30 artistas variados (use os dados abaixo)

```
Artistas para inserir:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
1. Adele - Pop
2. Arctic Monkeys - Indie Rock
3. Beatles - Rock
4. Beyoncé - R&B
5. Billie Eilish - Pop
6. Bob Marley - Reggae
7. Bruno Mars - Pop
8. Coldplay - Alternative Rock
9. David Bowie - Rock
10. Dua Lipa - Pop
11. Ed Sheeran - Pop
12. Elton John - Pop Rock
13. Eminem - Hip Hop
14. Foo Fighters - Rock
15. Frank Sinatra - Jazz
16. Guns N' Roses - Hard Rock
17. Imagine Dragons - Pop Rock
18. Iron Maiden - Heavy Metal
19. Jay-Z - Hip Hop
20. Jimi Hendrix - Rock
21. John Mayer - Blues Rock
22. Justin Timberlake - Pop
23. Kanye West - Hip Hop
24. Kendrick Lamar - Hip Hop
25. Led Zeppelin - Hard Rock
26. Madonna - Pop
27. Maroon 5 - Pop Rock
28. Metallica - Heavy Metal
29. Michael Jackson - Pop
30. Nirvana - Grunge
```

### Execução do Experimento:
1. Selecionar opção **12** (Experimento de busca)
2. Quando solicitado, digitar os 30 nomes dos artistas cadastrados
3. **Observar**:
   - Caminho percorrido na árvore para cada busca
   - Tempo gasto em cada busca
   - Estatísticas finais

### Resultados Esperados:
```
=== ANÁLISE DOS RESULTADOS ===
Total de buscas: 30
Artistas encontrados: 30
Artistas não encontrados: 0
Tempo total: X.XXXXXX ms
Tempo médio por busca: X.XXXXXX ms
Taxa de sucesso: 100.00%

=== INTERPRETAÇÃO ===
A Árvore 2-3 mantém balanceamento perfeito, garantindo que todas
as buscas tenham complexidade O(log n). O tempo médio de busca
depende da altura da árvore, que é sempre balanceada.
```

---

## 🧪 TESTE 9: Validação de Dependências

### Objetivo
Verificar que o sistema valida corretamente as dependências.

### Teste 9.1: Tentar inserir álbum em artista inexistente
```
Opção: 3 (Inserir álbum)
Nome do artista: Artista Inexistente
Título do álbum: Álbum Qualquer
Ano de lançamento: 2020

Resultado esperado:
❌ Artista 'Artista Inexistente' não encontrado!
```

### Teste 9.2: Tentar inserir música em álbum inexistente
```
Opção: 5 (Inserir música)
Nome do artista: The Beatles
Título do álbum: Álbum Inexistente
Título da música: Música Qualquer
Duração: 3.00

Resultado esperado:
❌ Álbum 'Álbum Inexistente' não encontrado!
```

---

## 📈 TESTE 10: Balanceamento da Árvore

### Objetivo
Verificar visualmente o balanceamento após múltiplas inserções.

### Passos:
1. Com os 30 artistas inseridos, selecionar opção **11**
2. **Observar**:
   - A árvore está balanceada?
   - Todos os caminhos da raiz até as folhas têm a mesma altura?
   - Os nós têm 1 ou 2 chaves conforme esperado?

### Resultado Esperado:
- Estrutura hierárquica clara
- Balanceamento perfeito (todas as folhas no mesmo nível)
- Nós com 1 ou 2 artistas cada

---

## ✅ CHECKLIST FINAL DE VALIDAÇÃO

Após executar todos os testes, verificar:

- [ ] ✅ Inserção de artistas funciona corretamente
- [ ] ✅ Inserção de álbuns associa ao artista correto
- [ ] ✅ Inserção de músicas mantém ordem alfabética
- [ ] ✅ Busca de artista retorna informações corretas
- [ ] ✅ Busca de álbum mostra músicas ordenadas
- [ ] ✅ Busca de música específica funciona
- [ ] ✅ Remoção de música individual funciona
- [ ] ✅ Remoção de álbum remove todas as músicas
- [ ] ✅ Remoção de artista remove toda a hierarquia
- [ ] ✅ Sistema valida dependências (não insere órfãos)
- [ ] ✅ Lista de artistas mostra ordem alfabética
- [ ] ✅ Hierarquia da árvore está balanceada
- [ ] ✅ Experimento de 30 buscas funciona
- [ ] ✅ Tempos de busca são medidos corretamente
- [ ] ✅ Caminho percorrido é exibido corretamente
- [ ] ✅ Estatísticas do experimento estão corretas
- [ ] ✅ Programa não trava ou apresenta erros
- [ ] ✅ Mensagens de erro são claras e apropriadas
- [ ] ✅ Interface é amigável e intuitiva

---

## 📝 NOTAS PARA O RELATÓRIO

### Pontos a observar e documentar:

1. **Altura da árvore**: Com 30 artistas, qual a altura da árvore?
2. **Tempo médio**: Qual o tempo médio de busca?
3. **Balanceamento**: Como a árvore se reorganiza após inserções?
4. **Comparação**: Como a Árvore 2-3 se compara à Red-Black (Questão 1)?

### Capturas sugeridas:
- 📸 Hierarquia da árvore com 7 artistas
- 📸 Hierarquia da árvore com 30 artistas
- 📸 Resultado do experimento de busca
- 📸 Listagem ordenada de artistas
- 📸 Exemplo de busca mostrando caminho percorrido

---

## 🎯 RESULTADOS ESPERADOS

### Complexidade Observada:
- **Busca**: O(log n) - proporcional à altura da árvore
- **Inserção**: O(log n) - incluindo possíveis quebras de nós
- **Remoção**: O(log n) - incluindo possíveis fusões

### Vantagens da Árvore 2-3:
- ✅ Balanceamento perfeito garantido
- ✅ Sem rotações complexas
- ✅ Altura uniforme (todas as folhas no mesmo nível)
- ✅ Operações garantidamente O(log n)

### Desvantagens:
- ⚠️ Implementação mais complexa que BST simples
- ⚠️ Overhead de memória (nós podem ter 1 ou 2 chaves)
- ⚠️ Quebra e fusão de nós tem custo

---

## 📊 TEMPLATE PARA RESULTADOS

```
=== TESTE EXECUTADO EM: __/__/____  ===

Número de artistas inseridos: ____
Número de álbuns inseridos: ____
Número de músicas inseridas: ____

Altura da árvore: ____ níveis

Experimento de 30 buscas:
- Tempo total: ______ ms
- Tempo médio: ______ ms
- Taxa de sucesso: _____%

Observações:
_________________________________
_________________________________
_________________________________
```

---

**🎓 Boa sorte com os testes! Este roteiro cobre todas as funcionalidades implementadas.**
