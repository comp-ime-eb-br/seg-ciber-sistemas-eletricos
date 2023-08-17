# seg-ciber-sistemas-eletricos
Este repositório contém os códigos em linguagem C de ataques cibernéticos realizados em um simulação de um sistema elétrico com a utilização de um Real-Time Digital Simulator (RTDS) e um IED comercial com o objetivo de se estudar a segurança cibernética nos ambientes de infraestruturas críticas de sistemas elétricos de potência.

## Ataques cibernéticos
A digitalização dos sistemas elétricos, promovida principalmente pela advento o conjunto de normas 61850 da International Eletrotechnical Comission (IEC 61850), trouxeram as ameaças cibernéticas a esses ambientes. Na literatura acadêmica, verifica-se diversos ataques cibernéticos possíveis que tomam proveito das vulnerabilidades do protocolo GOOSE da norma IEC 61850. Esses ataques são o ataque de retransmissão, de injeção de mensagem, de mascaramento e de envenamento [Quincozes et al. 2021].

O ataque de retransmissão é realizado com o reenvio de mensagens legítimas previamente enviadas.  Esse ataque tem o potencial de causar o funcionamento errôneo quando mesangens antigas com comandos realizados em determinado contexto de porteção e controle são reenviadas em outro contexto causando a realização de comandos.

O ataque de mascaramento é realizado a partir da captura de uma mensagem previamente enviada e seu conteúdo é modificado para realizar algum objetivo específico do atacante cibernético. Porém, nesse tipo de ataque, os parâmetros de sequenciamento das mensagens do protocolo GOOSE como o sqnum e stnum devem possuir coerência com o tráfego em tempo real, isso com o objetivo de dificultar medidas de detecção de intrusão que venham realizam a verificação do sequenciamento das mensagens.

Já o ataque de envenenamento é realizado com o objetivo de degradar a comunicação de rede entre os equipamentos da subestação. Esa degradação pode ser realizada através do inundamento de pacotes GOOSE na rede, causando a negação de serviço pelo mau funcionamento da comunicação. 


### Implementação dos Ataques cibernéticos

Os ataques cibernéticos foram implementados com a utilização da biblioteca open-source libiec61850 (https://libiec61850.com/), que provê interface servidor e cliente para comunicação em protocolos MMS, GOOSE e SV da norma 61850 em linguagem C. O autor da biblioteca libiec61850 informa que o objetivo do seu projeto é prover implementação que tenha portabilidade para ser usado em sistemas embarcados e também disponibiliza exemplos para serem usados como ponto de partida para a criação de aplicações [libiec61850 2022].

A partir dos exemplos de códigos de publicador e assinante em protocolo GOOSE disponibilizados na libiec61850, os códigos dos ataques foram gerados em linguagem C e estão disponíveis neste repositório com os comentários pertinentes para entedimento das soluções.


