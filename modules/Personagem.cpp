#define MAX_X 144
#define SPRITE_WIDTH 48
#define SPRITE_HEIGHT 64

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>
#include "./.env.h"

using namespace std;

class Monstro {
protected:
	string nome;
	int _HP, _ATQ, _DEF;
	IndiceMonstros _TIPOMONSTRO;
	int vidaAtual = 0;

public:
	Monstro(string nome, int ptVida, int ptDef, int ptAtq, IndiceMonstros tipoMonstro) : nome(nome), _HP(ptVida), _DEF(ptDef), _ATQ(ptAtq), _TIPOMONSTRO(tipoMonstro) {
		this->vidaAtual = ptVida;
	}

	// Função que verifica se o monstro está vivo, comparando se sua vida é maior que 0.
	bool verificaVivo() {
		if (this->vidaAtual > 0) {
			return true;
		}
		else {
			return false;
		}
	}

	// Função getter para o nome do monstro.
	string getNomeMonstro() {
		return this->nome;
	}

	// Função getter para a defesa do monstro.
	int getDefesaMonstro() {
		return this->_DEF;
	}

	// Função getter para o tipo do monstro.
	IndiceMonstros getTipoMonstro() {
		return _TIPOMONSTRO;
	}

	// Função getter que retorna um vetor com as estatísticas do monstro.
	int* getEstatisticas(bool valoresAtuais = false) {
		int* estatisticas = new int[4];

		if (valoresAtuais)
			estatisticas[0] = this->vidaAtual;
		else
			estatisticas[0] = this->_HP;
		estatisticas[1] = this->_DEF;
		estatisticas[2] = this->_ATQ;

		return estatisticas;
	}

	// Função que imprime as estatísticas do monstro.
	void printDetalhesMonstro(bool valoresAtuais = false) {
		int* estatisticas = this->getEstatisticas(valoresAtuais);

		vector<string> cabecalho = { "Estatisticas:\n", "" };
		vector<vector<string>> tabela = {
		  {"Nome:", this->nome},
		  {"Vida:", to_string(estatisticas[0])},
		  {"Defesa:", to_string(estatisticas[1])},
		  {"Dano de ataque:", to_string(estatisticas[2])},
		};

		delete[] estatisticas;

		std::cout << "\n" << cabecalho[0] << cabecalho[1] << endl;
		for (int i = 0; i < tabela.size(); i++) {
			cout << tabela[i][0] << " " << tabela[i][1] << endl;
		}
	}


	// Função que faz o monstro tomar dano.
	void tomarDano(int dano) {
		if (dano <= this->_DEF) {
			cout << "O monstro defendeu o ataque!" << endl;
			return;
		}

		this->vidaAtual -= (dano - this->_DEF);
	}

	// Função que faz o monstro atacar o jogador.
	int atacar(int defesaJogador) {
		int dano = this->_ATQ - defesaJogador;
		cout << "\nO " << COR_VERMELHA << "monstro" << RESET_COR << " causou " << dano << " de dano!" << endl;

		return this->_ATQ;
	}

	// Função que imprime a vida do monstro.
	void vidaMonstro() {
		int vida = this->vidaAtual < 0 ? 0 : this->vidaAtual;
		cout << "Vida do monstro: " << vida << "/" << this->_HP << endl;
	}
};

class Aranha : public Monstro {
public:
	Aranha() : Monstro("Aranha", ARANHA_VIDA, ARANHA_DEFESA, ARANHA_ATAQUE, IndiceMonstros::ARANHA) {}
};

class AranhaGrande : public Monstro {
public:
	AranhaGrande() : Monstro("Aranha Grande", ARANHA_GRANDE_VIDA, ARANHA_GRANDE_DEFESA, ARANHA_GRANDE_ATAQUE, IndiceMonstros::ARANHA_GRANDE) {}
};

class Escorpiao : public Monstro {
public:
	Escorpiao() : Monstro("Escorpiao", ESCORPIAO_VIDA, ESCORPIAO_DEFESA, ESCORPIAO_ATAQUE, IndiceMonstros::ESCORPIAO) {}
};

class Personagem {

private:
	int _HP, _ATQ, _DEF, _ESP, _CUSTO_MANA;
	int vidaAtual = 0;
	int manaAtual = 0;
	int grauFerimento = 0;
	int itensAd = TOTAL_ITENS_ADQUIRIDOS;
	int ultArm = -1;
	int armAtual = 0;
	vector<string> equipamentos = { "Picareta" };

	sf::Texture texture;
	sf::IntRect rectSourceSprite;
	sf::Sprite sprite;
	Direcoes direcaoAtual;

	int posicaoAtualX = 0;
	int posicaoAtualY = 0;

	int minerios[4][2] = {
	  {OURO, 0},
	  {MORKITA, 0},
	  {DYSTRUM, 0},
	  {NITRA, 0}
	};

public:
	Personagem(int ptVida, int ptDef, int ptAtq, int ptAtqEsp, int mana, int classePersonagem) : _HP(ptVida), _ATQ(ptAtq), _ESP(ptAtqEsp), _DEF(ptDef), _CUSTO_MANA(mana) {
		this->vidaAtual = ptVida;
		direcaoAtual = BAIXO;
		
		if (classePersonagem == BATEDOR) {
			this->texture.loadFromFile(ASSETS_FOLDER + "sprites/personagem/batedor.png");
		}
		else if (classePersonagem == GUERREIRO) {
			this->texture.loadFromFile(ASSETS_FOLDER + "sprites/personagem/guerreiro.png");
		}
		else if (classePersonagem == ENGENHEIRO) {
			this->texture.loadFromFile(ASSETS_FOLDER + "sprites/personagem/engenheiro.png");
		}
		else if (classePersonagem == ESCAVADOR) {
			this->texture.loadFromFile(ASSETS_FOLDER + "sprites/personagem/escavador.png");
		}
		else if (classePersonagem == MEDICO) {
			this->texture.loadFromFile(ASSETS_FOLDER + "sprites/personagem/medico.png");
		}
		else {
			return;
		}
		this->rectSourceSprite = sf::IntRect(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
		this->sprite = sf::Sprite(this->texture, this->rectSourceSprite);
		this->sprite.setPosition(static_cast<float>(50), static_cast<float>(50));
	}

	void animateWalk(Direcoes direcao) {
		switch (direcao)
		{
		case ESQUERDA:
			if (direcaoAtual != ESQUERDA) rectSourceSprite.left = 0;
			direcaoAtual = ESQUERDA;

			rectSourceSprite.top = SPRITE_HEIGHT * 3;
			break;
		case DIREITA:
			if (direcaoAtual != DIREITA) rectSourceSprite.left = 0;
			direcaoAtual = DIREITA;

			rectSourceSprite.top = SPRITE_HEIGHT * 1;
			break;
		case CIMA:
			if (direcaoAtual != CIMA) rectSourceSprite.left = 0;
			direcaoAtual = CIMA;

			rectSourceSprite.top = SPRITE_HEIGHT * 0;
			break;
		case BAIXO:
			if (direcaoAtual != BAIXO) rectSourceSprite.left = 0;
			direcaoAtual = BAIXO;

			rectSourceSprite.top = SPRITE_HEIGHT * 2;
			break;
		default:
			break;
		}

		if (rectSourceSprite.left == (MAX_X - SPRITE_WIDTH))
			rectSourceSprite.left = 0;
		else
			rectSourceSprite.left += SPRITE_WIDTH;
	}

	sf::Sprite getCurrentSprite() {
		this->sprite.setTextureRect(this->rectSourceSprite);

		return this->sprite;
	}

	void move(const sf::Vector2f& offset, const sf::Vector2u& windowSize) {

		// Verifique se o novo local nao ultrapassa os limites da janela
		sf::Vector2f newPosition = sprite.getPosition() + offset;
		if (newPosition.x >= 0 && newPosition.x <= windowSize.x - SPRITE_WIDTH &&
			newPosition.y >= 0 && newPosition.y <= windowSize.y - SPRITE_HEIGHT) {
			sprite.move(offset);

			this->posicaoAtualX = newPosition.x;
			this->posicaoAtualY = newPosition.y;
		}
	}

	sf::Vector2f getPosition() const {
		return sprite.getPosition();
	}

	void setPosition(const sf::Vector2f& position) {
		sprite.setPosition(position);
	}

	// Função responsável por dar ao jogador os minérios que ele dropou do monstro.
	void recebeMinerios(int minerio, int quantidade) {
		this->minerios[minerio][1] += quantidade;
	}

	// Função getter para a defesa do jogador.
	int getDefesaPersonagem() {
		return this->_DEF;
	}

	// Função que verifica se o jogador está vivo, comparando se sua vida é maior que 0.
	bool verificaVivo() {
		if (this->vidaAtual > 0) {
			return true;
		}
		else {
			return false;
		}
	}

	// Função que verifica se o jogador tem mana suficiente para executar um ataque especial, comparando se sua mana atual é maior que o custo.
	// Caso não seja, o jogador recebe 1/4 do custo de mana.
	bool VerificaMana() {
		if (this->manaAtual >= this->_CUSTO_MANA) {
			return true;
		}
		else {
			this->manaAtual += (this->_CUSTO_MANA / 4);
			return false;
		}
	}

	// Função responsável por solicitar uma cápsula de cura ao jogador, ao custo de NITRA.
	void solicitaCapsula() {
		if (this->minerios[NITRA][1] >= CUSTO_CAPSULA) {
			this->vidaAtual += int((this->_HP / 2));

			if (this->vidaAtual > this->_HP) this->vidaAtual = this->_HP;

			this->grauFerimento = 0;
			cout << "Voce se curou em " << int((this->_HP / 2)) << " pontos de vida e curou suas feridas!" << endl;
		}
		else {
			cout << "Voce nao possui nitra suficiente para chamar uma capsula de cura." << endl;
		}
	}

	// Função getter que retorna um vetor com as estatísticas do jogador.
	int* getEstatisticas(bool valoresAtuais = false) {
		int* estatisticas = new int[5];

		if (valoresAtuais)
			estatisticas[0] = this->vidaAtual;
		else
			estatisticas[0] = this->_HP;

		estatisticas[1] = this->_DEF;
		estatisticas[2] = this->_ATQ;
		estatisticas[3] = this->_ESP;
		estatisticas[4] = this->grauFerimento;

		return estatisticas;
	}

	// Função que imprime as estatísticas do jogador e seus minérios.
	void printDetalhesClasse(bool valoresAtuais = false) {
		int* estatisticas = this->getEstatisticas(valoresAtuais);

		vector<string> cabecalho = { "Estatisticas:", "\n" };
		vector<vector<string>> tabela = {
			{"Nome:", "Analise de Sistemas"},
			{"Vida:", to_string(estatisticas[0])},
			{"Defesa:", to_string(estatisticas[1])},
			{"Dano de ataque:", to_string(estatisticas[2])},
			{"Dano do especial:", to_string(estatisticas[3])},
			{"Grau de sangramento: ", to_string(estatisticas[4])}
		};

		delete[] estatisticas;

		std::cout << "\n" << cabecalho[0] << cabecalho[1] << cabecalho[2] << cabecalho[3] << endl;
		for (int i = 0; i < tabela.size(); i++) {
			cout << tabela[i][0] << " " << tabela[i][1] << " " << tabela[i][2] << " " << tabela[i][3] << endl;
		}
	}

	int atacar(Monstro* monstro) {
		int escolha = NAO_ESPECIFICADO;

		// Mostra as opes e obtm a escolha do jogador
		while (escolha == NAO_ESPECIFICADO) {
			cout << endl << endl << "Escolha uma opcao:" << endl << endl;
			cout << "1 - Atacar" << " (Dano atual do ataque: " << _ATQ << ")" << endl << "2 - Atacar Especial" << endl << "3 - Trocar arma" << endl << "4 - Ver seus atributos" << endl << "5 - Ver atributos do monstro" << endl << "Escolha:";
			cin >> escolha;

			// Caso a escolha do jogador no seja nenhuma das opes, ele  solicitado a escolher novamente.
			if (escolha != NORMAL && escolha != ESPECIAL && escolha != INFORMACOES_PLAYER && escolha != INFORMACOES_MONSTRO && escolha != CAPSULA_CURA && escolha != TROCAR_ARMA) {
				cout << "Escolha invalida, por favor escolha entre ataque NORMAL ou ESPECIAL!!" << endl;
				escolha = NAO_ESPECIFICADO;
			}

			else if (escolha == TROCAR_ARMA) {
				system("clear||cls");
				printItensInventario();
				escolha = NAO_ESPECIFICADO;
			}

			// Caso a escolha seja INFORMACOES_PLAYER, mostramos os atributos do jogador e resetamos o loop.
			else if (escolha == INFORMACOES_PLAYER) {
				system("clear||cls");
				printDetalhesClasse(true);
				escolha = NAO_ESPECIFICADO;
			}

			 //Caso a escolha seja INFORMACOES_MONSTRO, mostramos os atributos do monstro e resetamos o loop.
			else if (escolha == INFORMACOES_MONSTRO) {
				system("clear||cls");
				monstro->printDetalhesMonstro(true);
				escolha = NAO_ESPECIFICADO;
			}

			// Caso a escolha seja CAPSULA_CURA, tentamos curar o jogador e resetamos o loop.
			else if (escolha == CAPSULA_CURA) {
				system("clear||cls");
				solicitaCapsula();
				escolha = NAO_ESPECIFICADO;
			}

			cout << endl;
		}

		// Por fim, calculamos o dano de ataque do jogador, verificamos sua mana e verificamos o ataque escolhido.
		int dano = this->_ATQ - monstro->getDefesaMonstro();
		bool manaSuficiente = this->VerificaMana();

		system("clear||cls");

		// Se for diferente de normal, quer dizer que  o especial
		if (escolha != Escolha::NORMAL) {
			if (manaSuficiente) {
				// O nmero aleatrio aqui  para simular um ataque crtico
				std::random_device                  rand_dev;
				std::mt19937                        generator(rand_dev());
				std::uniform_int_distribution<int>  distr(0, 4);

				int ran = distr(generator);

				// O dano  composto pelo especial menos a defesa do monstro
				dano = this->_ESP - monstro->getDefesaMonstro();

				// Se o nmero aleatrio for 2, o ataque  crtico e  multiplicado por 2
				if (ran == 2) {
					cout << "ATAQUE CRTICO!\n";
					dano = this->_ESP * 2;
				}

				// A mana  diminuda pelo custo do especial
				this->manaAtual -= this->_CUSTO_MANA;

				// O dano  mostrado na tela
				cout << COR_VERDE << "\nVoc " << RESET_COR << "causou " << dano << " de dano!" << endl;
				return this->_ESP;
			}
			else {
				cout << "O ataque especial nao esta pronto! Voce trocou para o ataque normal!" << endl;
			}
		}

		cout << COR_VERDE << "\nVoce " << RESET_COR << "causou " << dano << " de dano!" << endl;
		return this->_ATQ;
	}

	// Função que faz o jogador tomar dano.
	void tomarDano(int dano) {
		if (dano <= this->_DEF) {
			cout << "Voce defendeu o ataque!" << endl;
			return;
		}

		this->vidaAtual -= (dano - this->_DEF);
	}

	// Função que faz o jogador ser ferido pelo monstro (sangramento).
	void aumentaFerimento() {
		cout << "Voce foi ferido!" << endl;

		this->grauFerimento += 1;
	}

	// Função que calcula o dano do sangramento.
	int danoFerimento() {
		return DANO_SANGRAMENTO * this->grauFerimento;
	}

	// Função que faz o jogador sangrar, caso ele esteja ferido.
	void sangrar(int dano) {
		if (this->grauFerimento == 0) return;

		cout << "\nVoce está sangrando!" << endl;
		cout << "Voce recebeu " << dano << " de dano!\n" << endl;
		this->vidaAtual -= dano;

		if (this->vidaAtual <= 0) {
			this->morrer();
		}
	}

	// Função que imprime a vida do jogador.
	void vidaPersonagem() {
		int vida = this->vidaAtual < 0 ? 0 : this->vidaAtual;
		cout << "Sua vida: " << vida << "/" << this->_HP << endl;
	}

	// Função que imprime a morte do jogador.
	void morrer() {
		cout << "Voce morreu!" << endl;
	}

	void printItensInventario() {
		int escolha = 0;
		cout << "Armas no invetrio:" << endl << endl;
		for (int i = 0;i < equipamentos.size();i++) {
			cout << i + 1 << " - " << equipamentos[i];

			switch (i - 1)
			{
			case UZI:
				cout << ", Dano: " << DANO_UZI << endl;
				break;
			case METRALHADORA:
				cout << ", Dano: " << DANO_METRALHADORA << endl;
				break;
			case SHOOTGUN:
				cout << ", Dano: " << DANO_SHOOTGUN << endl;
				break;
			default:
				cout << ", Dano: " << DANO_PICARETA << endl;
				break;
			}
		}

		cout << "Escolha uma arma: ";
		cin >> escolha;

		if (escolha > equipamentos.size()) {
			cout << "Escolha invalida!" << endl;
			return;
		}
		else {
			this->armAtual = escolha;
			cout << endl << "Escolha: " << escolha << endl;
			switch (escolha-2)
			{
			case UZI:
				this->_ATQ = DANO_UZI;
				break;
			case METRALHADORA:
				this->_ATQ = DANO_METRALHADORA;
				break;
			case SHOOTGUN:
				this->_ATQ = DANO_SHOOTGUN;
				break;
			default:
				this->_ATQ = DANO_PICARETA;
				break;
			}
			cout << endl << "Sua arma atual foi alterada!";
		}
	}

	void addItem(int item) {
		if (item == SHOOTGUN) {
			equipamentos.push_back("ShootGun");
			ultArm = item;
		}
		else if (item == METRALHADORA) {
			equipamentos.push_back("Metralhadora");
			ultArm = item;
		}
		else if (item == UZI) {
			equipamentos.push_back("Uzi");
			ultArm = item;
		}
	}

	int getUltArm() {
		return ultArm;
	}
};

class Batedor : public Personagem {
public:
	Batedor(int classe) : Personagem(BATEDOR_VIDA, BATEDOR_DEFESA, DANO_PICARETA, BATEDOR_ATQ_ESPECIAL, BATEDOR_MANA, classe) {}
};

class Guerreiro : public Personagem {
public:
	Guerreiro(int classe) : Personagem(GUERREIRO_VIDA, GUERREIRO_DEFESA, DANO_PICARETA, GUERREIRO_ATQ_ESPECIAL, GUERREIRO_MANA, classe) {}
};

class Engenheiro : public Personagem {
public:
	Engenheiro(int classe) : Personagem(ENGENHEIRO_VIDA, ENGENHEIRO_DEFESA, DANO_PICARETA, ENGENHEIRO_ATQ_ESPECIAL, ENGENHEIRO_MANA, classe) {}
};

class Escavador : public Personagem {
public:
	Escavador(int classe) : Personagem(ESCAVADOR_VIDA, ESCAVADOR_DEFESA, DANO_PICARETA, ESCAVADOR_ATQ_ESPECIAL, ESCAVADOR_MANA, classe) {}
};

class Medico : public Personagem {
public:
	Medico(int classe) : Personagem(MEDICO_VIDA, MEDICO_DEFESA, DANO_PICARETA, MEDICO_ATQ_ESPECIAL, MEDICO_MANA, classe) {}
};