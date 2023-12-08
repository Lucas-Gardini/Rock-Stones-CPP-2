#ifndef ENV_H
#define ENV_H

#include <string>

// Configurações do projeto
const std::string ASSETS_FOLDER = "./assets/";

// Cores para o terminal.
const std::string COR_VERMELHA = "\033[1;31m";
const std::string COR_VERDE = "\033[1;32m";
const std::string RESET_COR = "\033[0m";

enum Direcoes {
	ESQUERDA,
	DIREITA,
	CIMA,
	BAIXO
};

// Tipos de classes e seus nomes.
enum Classe {
	BATEDOR,
	GUERREIRO,
	ENGENHEIRO,
	ESCAVADOR,
	MEDICO
};

const std::string CLASSES[5] = { "Batedor", "Guerreiro", "Engenheiro", "Escavador", "Medico" };

// Tipos de escolha e seções do jogo.
enum Escolha {
	NAO_ESPECIFICADO,
	NORMAL,
	ESPECIAL,
	INFORMACOES_PLAYER,
	INFORMACOES_MONSTRO,
	CAPSULA_CURA
};

enum Secoes {
	SECAO_INDEFINIDA,
	BATALHA,
	EXTRATOR
};

const std::string SECOES[3] = { "Seção Indefinida", "Batalha", "Extrator" };

// Tipos de minérios e seus nomes.
enum Minerios {
	OURO,
	MORKITA,
	DYSTRUM,
	NITRA
};

const std::string MINERIOS[4] = { "Ouro", "Morkita", "Dystrum", "Nitra" };

// TIpos de monstros.
enum IndiceMonstros {
	ARANHA,
	ARANHA_GRANDE,
	ESCORPIAO
};

// Constantes do jogo.
const int CUSTO_CAPSULA = 100;
const int CURA_CAPSULA = 20;

const int DANO_SANGRAMENTO = 2;

const int BATEDOR_VIDA = 60;
const int BATEDOR_ATAQUE = 20;
const int BATEDOR_DEFESA = 0;
const int BATEDOR_ATQ_ESPECIAL = 42;
const int BATEDOR_CUSTO_ESPECIAL = 110;
const int BATEDOR_MANA = 200;

const int GUERREIRO_VIDA = 100;
const int GUERREIRO_ATAQUE = 27;
const int GUERREIRO_DEFESA = 7;
const int GUERREIRO_ATQ_ESPECIAL = 35;
const int GUERREIRO_CUSTO_ESPECIAL = 100;
const int GUERREIRO_MANA = 200;

const int ENGENHEIRO_VIDA = 70;
const int ENGENHEIRO_ATAQUE = 25;
const int ENGENHEIRO_DEFESA = 2;
const int ENGENHEIRO_ATQ_ESPECIAL = 35;
const int ENGENHEIRO_CUSTO_ESPECIAL = 80;
const int ENGENHEIRO_MANA = 200;

const int ESCAVADOR_VIDA = 80;
const int ESCAVADOR_ATAQUE = 20;
const int ESCAVADOR_DEFESA = 3;
const int ESCAVADOR_ATQ_ESPECIAL = 35;
const int ESCAVADOR_CUSTO_ESPECIAL = 80;
const int ESCAVADOR_MANA = 200;

const int MEDICO_VIDA = 60;
const int MEDICO_ATAQUE = 20;
const int MEDICO_DEFESA = 4;
const int MEDICO_ATQ_ESPECIAL = 30;
const int MEDICO_CUSTO_ESPECIAL = 70;
const int MEDICO_MANA = 200;

const int ARANHA_VIDA = 20;
const int ARANHA_ATAQUE = 8;
const int ARANHA_DEFESA = 2;

const int ARANHA_GRANDE_VIDA = 35;
const int ARANHA_GRANDE_ATAQUE = 13;
const int ARANHA_GRANDE_DEFESA = 3;

const int ESCORPIAO_VIDA = 23;
const int ESCORPIAO_ATAQUE = 15;
const int ESCORPIAO_DEFESA = 5;

#endif
