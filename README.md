# TMID

## 1: Instalação
dentro da pasta TMID/, lança esses comandos

### 1.1 Builda o executavel do programa
$ mkdir build
$ g++ src/main src/task_commands.cpp src/group_commands.cpp -o build/tmid

### 1.2 Builda e executa o instalador
$ g++ tmid-install.cpp -o tmid-install
$ ./sudo tmid-install

## 2: funções

### 3.1: implementadas
tmid create group - cria um config_file pro grupo na pasta atual
tmid create task - cria uma config_file pra tarefa na pasta atual

### 3.2: Ainda nao implementadas
tmid update - atualiza a lista de tarefas para fazer
tmid check - checa se todos os arquivos necessários estao presentes
