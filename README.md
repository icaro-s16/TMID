# TMID

## 1: Instalação
dentro da pasta TMID/, lança esses comandos

### 1.1 Builda o executavel do programa
1. `mkdir build`
2. `g++ src/main src/task_commands.cpp src/group_commands.cpp -o build/tmid`

### 1.2 Builda e executa o instalador
3. `g++ tmid-install.cpp -o tmid-install`
4. `./sudo tmid-install`


## 2: funções
### 3.1: implementadas
- tmid create group /* cria um config_file para um grupo na pasta atual */
- tmid create task  /* cria um config_file para uma tarefa na pasta atual */

### 3.2: Ainda nao implementadas
- tmid update /* atualiza a lista de tarefas para fazer */
- tmid check  /* checa se todos os arquivos necessários estao presentes */
