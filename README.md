# Implementação de um sistema com região mútua distribuída
### Universidade Federal do Rio de Janeiro (UFRJ) 2023.1
### Sistemas Distribuídos COS470 - Trabalho Prático 3

> Como compilar o código? 
```
g++ coordenador.cpp Spinlock.cpp message.cpp udpSender.cpp udpReceiver.cpp queue.cpp getTime.cpp -o coordenador && g++ process.cpp message.cpp udpSender.cpp udpReceiver.cpp getTime.cpp -o process
```

> Como rodar? 
1. Rode o coordenador `./coordenador`.
2. Rode os processos participantes utilizando o script em shell, `./init.sh`. Se quiser, edite o arquivo e mude os parâmetros dos processos.
