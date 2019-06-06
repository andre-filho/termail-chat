# TERMAIL - chat de terminal de FSO
------

## Informações sobre até qual parte do protocolo foi implementado
Com exceção do broadcast, todas as outras funcionalidades foram implementadas.

## Problemas conhecidos
Tivemos problemas nos testes de retry com a fila cheia.

Interface não está muito bonita e as palavras quebram quando se esta escrevendo e se recebe uma mensagem.

## Dificuldades de implementação do modelo de threads
- UMASK;

- excessões de quando a filas do receptor não
existe;

- acesso às filas com threads;

- argumentos de acesso das filas;



------
# Como rodar o programa
```bash
$ make clean

$ make

$ make run

$ ./bin/exe
```
