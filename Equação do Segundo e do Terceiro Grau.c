#include <stdio.h>

#include <stdlib.h>

#include <locale.h> //Biblioteca para mostrar acentuação.


//ESTRUTURAS
typedef struct { //estrutura para equações de até terceiro grau
  double a, b, c, d, delta; //valores constantes da equação
  double raizes2[2]; //vetor armazenando as raízes do segundo grau ou raízes da derivada da função do terceiro grau
  double raizes3[3]; //vetor armazenando as raízes do terceiro grau
  int r3; //número de raízes da equação do terceiro grau
}
infoequacao;

//FUNÇÕES EQUIVALENTES À RESOLUÇÃO DE EQUAÇÕES DO SEGUNDO OU TERCEIRO GRAU
infoequacao derivada(infoequacao x); //função para retornar a derivada de uma função do segundo ou terceiro grau
infoequacao derivada2(infoequacao x); //função para retornar a derivada segunda de uma função do terceiro grau
double encontrarintervalo(infoequacao x, double a, double i); //função para encontrar o valor correto a ser aplicado no método da bisseção
void equa(); //menu para chamar uma função de resolução de uma equação do segundo ou terceiro grau
infoequacao equa2(infoequacao x); //preencher informações de uma equação do segundo grau
infoequacao equa3(infoequacao x); //prencher informações de uma equação do terceiro grau
infoequacao metodobissec(infoequacao x, int i, double a, double b); //método da bisseção para encontrar o f(x) = 0 num intervalo entre a e b
double sqr(double x); //função para extrair a raiz quadrada de um número
double yfuncao(infoequacao equacao, double x); //função para calcular o y para um determinado x, caso o int D seja diferente de 0, será calculado o y da derivada

int main() {

  //Linha para adequar acentução.
  setlocale(LC_ALL, "Portuguese");

  equa();

  return 0;

}

infoequacao derivada(infoequacao x) { //função da derivada primeira
  x.d = x.c;
  x.c = x.b * 2;
  x.b = x.a * 3;
  x.a = 0;
  return x;
}

infoequacao derivada2(infoequacao x) { //função da derivada segunda para funções do terceiro grau
  x = derivada(x);
  x.d = x.c;
  x.c = x.b * 2;
  x.b = 0;
  x.a = 0;
  return x;
}

double encontrarintervalo(infoequacao x, double a, double i) { //função para encontrar um intervalo coerente para utilizar a função metodobissec(); utiliza-se o 'a' para saber qual o primeiro intervalo e o i para saber em qual dire��o a função deve procurar o intervalo desejado
  double temp = a;
  while ((yfuncao(x, a) * yfuncao(x, temp)) > 0) {
    temp = temp + i;
    if ((yfuncao(x, a) * yfuncao(x, temp)) <= 0) { //adicional extra s� para garantir intervalo coerente
      temp = temp + i;
    }
  }
  return temp;
}

void equa() {
  infoequacao equacao;
  printf("\n\nDigite os coeficientes 'a','b','c' e 'd' da sua equação.\n"); //Aqui explica-se como calcular a raiz da equação
  scanf(" %lf %lf %lf %lf", & equacao.a, & equacao.b, & equacao.c, & equacao.d);
  printf("Sua equação é: %lfx³ + %lfx² + %lfx + %lf = 0\n\n", equacao.a, equacao.b, equacao.c, equacao.d);
  if (equacao.a == 0) {
    equacao = equa2(equacao);
    if (equacao.delta < 0) {
      printf("Não existe raiz real.");
    } else if (equacao.delta == 0) {
      printf("\nExiste apenas uma raiz igual a %lf\n\n", equacao.raizes2[0]);
    } else {
      printf("\nRaiz 1 = %lf\nRaiz 2 = %lf\n\n", equacao.raizes2[0], equacao.raizes2[1]);
    }
  } else {
    equacao = equa3(equacao);
    printf("%i",equacao.r3);
    switch (equacao.r3) {
    case 1:
      printf("\n\nRaiz única:\n     %lf\n\n", equacao.raizes3[0]);
      break;
    case 2:
      printf("\n\nRaiz número 1:\n     %lf\n\n", equacao.raizes3[0]);
      printf("\n\nRaiz número 2:\n     %lf\n\n", equacao.raizes3[1]);
      break;
    case 3:
      printf("\n\nRaiz número 1:\n     %lf\n\n", equacao.raizes3[0]);
      printf("\n\nRaiz número 2:\n     %lf\n\n", equacao.raizes3[1]);
      printf("\n\nRaiz número 3:\n     %lf\n\n", equacao.raizes3[2]);
      break;
    }
  }
  //variável para saber o que fazer quando as raízes tiverem sido encontradas
  int continuar;
  invalid:
    printf("Digite:\n1 -> Descobrir raízes de outra equação;\n2 -> Encerrar programa;\n");
  scanf("%i", & continuar);
  if (continuar == 1) {
    equa();
  } else if (continuar == 2) {
    exit;
  } else {
    printf("\nValor inválido, digite '1' ou '2'.\n\n");
    goto invalid;
  }
}

infoequacao equa2(infoequacao x) {
  x.delta = x.c * x.c - 4 * x.b * x.d;
  if (x.delta < 0) {
    return x;
  }
  x.raizes2[0] = (-x.c - sqr(x.delta)) / (2 * x.b);
  x.raizes2[1] = (-x.c + sqr(x.delta)) / (2 * x.b);
  return x;
}

infoequacao equa3(infoequacao x) {
  double temp;
  x.raizes2[0] = (equa2(derivada(x))).raizes2[0];
  x.raizes2[1] = (equa2(derivada(x))).raizes2[1];
  if (x.raizes2[0] > x.raizes2[1]) {
    temp = x.raizes2[1];
    x.raizes2[1] = x.raizes2[0];
    x.raizes2[0] = temp;
  }
  //variável idr para verificar a quantidade de raízes com base na compara��o dos sinais das ordenadas dos pontos críticos
  double idr = yfuncao(x, x.raizes2[0]) * yfuncao(x, x.raizes2[1]);
  ///////////////////////////////////////////////////////
  //caso os sinais sejam opostos, a equação ter� 3 raízes
  ///////////////////////////////////////////////////////
  if (idr < 0) {

    //primeira raiz
    x = metodobissec(x, 0, x.raizes2[0], encontrarintervalo(x, x.raizes2[0], -1));

    //segunda raiz
    x = metodobissec(x, 1, x.raizes2[0], x.raizes2[1]);

    //terceira raiz
    x = metodobissec(x, 2, x.raizes2[1], encontrarintervalo(x, x.raizes2[1], 1));

    x.r3 = 3;

  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //caso os sinais sejam iguais, a equação ter� 1 raiz apenas, e esta raiz n�o coincidir� com um ponto de inflex�o
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  else if (idr > 0) {
    //verificando produto de f'(x) com f(x) para verificar sentido em que a raiz est� em rela��o ao seu ponto m�nimo, caso seja negativo, est� � esquerda do primeiro ponto cr�tico, caso seja positivo, � direita
    //subtrai-se 1 na f'(x) porque, caso contr�rio, a derivada dar� zero, por se tratar do ponto de inflex�o
    if (yfuncao(derivada(x), x.raizes2[0] - 1) * yfuncao(x, x.raizes2[0]) < 0) {
      x = metodobissec(x, 0, x.raizes2[0], encontrarintervalo(x, x.raizes2[0], 1));
    } else {
      x = metodobissec(x, 0, x.raizes2[1], encontrarintervalo(x, x.raizes2[1], -1));
    }
    x.r3 = 1;
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //caso o produto seja igual � 0, ou a equação possui 2 raízes e uma delas � um dos pontos críticos ou possui apenas uma raiz composta pelo ponto de inflex�o
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  else {
    //raiz no ponto de inflex�o
    if ((float) x.raizes2[0] == (float) x.raizes2[1]) {
      x.raizes3[0] = x.raizes2[0];
      x.r3 = 1;
    }
    //raiz em um dos pontos de m�nimo ou m�ximo
    else if ((float) yfuncao(x, x.raizes2[0]) == 0) {
      x.raizes3[0] = x.raizes2[0];
      x = metodobissec(x, 1, x.raizes2[1], encontrarintervalo(x, x.raizes2[1], 1));
      x.r3 = 2;
    } else if ((float) yfuncao(x, x.raizes2[1]) == 0) {
      x.raizes3[1] = x.raizes2[1];
      x = metodobissec(x, 0, x.raizes2[0], encontrarintervalo(x, x.raizes2[0], -1));
      x.r3 = 2;
    }
  }
  return x;
}

//x = equação a ser preenchida
//i = posição do vetor das raízes a ser preenchida
//a e b = intervalo no qual a raiz se encontra
infoequacao metodobissec(infoequacao x, int i, double a, double b) { //método para encontrar f(x) = 0 em um intervalo entre a e b em que se sabe com certeza que existe uma raiz
  double temp = (a + b) / 2; //o intervalo vai se reduzindo a cada vez que a função � chamada, de modo a encontrar a raiz por meio da itera��o da função
  if ((a - b) * (a - b) > 0.00000000001) {
    //verificador abaixo em caso de erro
    //printf("\n\ndouble a = %lf f(a) = %lf\ndouble b = %lf f(b) = %lf\ndouble temp = %lf f(temp) = %lf\n",a,yfuncao(x,a),b,yfuncao(x,b),temp,yfuncao(x,temp));
    if (yfuncao(x, a)) {
      if (yfuncao(x, a) * yfuncao(x, temp) < 0) {
        return metodobissec(x, i, a, temp);
      } else if (yfuncao(x, b) * yfuncao(x, temp) < 0) {
        return metodobissec(x, i, b, temp);
      }
    }
  }
  x.raizes3[i] = temp; //preenchimento da equação
  return x;
}

//raiz através do método de Newton
double sqr(double num) {

  double temp = 0, raiz = num / 2;

  while (raiz != temp) {
    temp = raiz;
    raiz = (num / temp + temp) / 2;
  }

  return raiz;
}

double yfuncao(infoequacao equacao, double x) {
  double y;

  y = equacao.a * x * x * x + equacao.b * x * x + equacao.c * x + equacao.d;

  return y;
}