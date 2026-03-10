# TMID

## 1: Instalação
dentro da pasta TMID/, execute os comandos especificados

### 1.1 compilar o executavel do programa e o instalador
1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `cmake --build .`

### 1.2 executar o instalador
4. `./sudo tmid-install`

## 2: funções
### 2.1: implementadas
- tmid create group /* cria um config_file para um grupo na pasta atual */
- tmid create task  /* cria um config_file para uma tarefa na pasta atual */
- tmid run          /* inicia o servidor para receber arquivos */
- tmid send         /* envia arquivos da pasta para o servidor */
- tmid update       /* atualiza a lista de tarefas para fazer */

### 2.2: Ainda nao implementadas
- tmid check  /* checa se todos os arquivos necessários estao presentes */
