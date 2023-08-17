# seg-ciber-sistemas-eletricos
Este repositório contém os códigos em linguagem C de ataques cibernéticos realizados em um simulação de um sistema elétrico com a utilização de um Real-Time Digital Simulator (RTDS) e um IED comercial com o objetivo de se estudar a segurança cibernética nos ambientes de infraestruturas críticas de sistemas elétricos de potência.

## Ataques cibernéticos
A digitalização dos sistemas elétricos, promovida principalmente pela advento o conjunto de normas 61850 da International Eletrotechnical Comission (IEC 61850), trouxeram as ameaças cibernéticas a esses ambientes. Na literatura acadêmica, verifica-se diversos ataques cibernéticos possíveis que tomam proveito das vulnerabilidades do protocolo GOOSE da norma IEC 61850. Esses ataques são o ataque de retransmissão, de injeção de mensagem, de mascaramento e de envenamento [Quincozes et al. 2021].


### Implementação dos Ataques cibernéticos

Os ataques cibernéticos foram implementados com a utilização da biblioteca open-source libiec61850 (https://libiec61850.com/), que provê interface servidor e cliente para comunicação em protocolos MMS, GOOSE e SV da norma 61850 em linguagem C. O autor da biblioteca libiec61850 informa que o objetivo do seu projeto é prover implementação que tenha portabilidade para ser usado em sistemas embarcados e também disponibiliza exemplos para fsserem usados como ponto de partida para a criação de aplicações [libiec61850 2022].
