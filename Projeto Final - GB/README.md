# Computação Gráfica - Guilherme Quadros e Bernardo Lorenzini

## Visualizador de Cenas 3D

Implementamos um visualizador de cenas 3D como trabalho final da cadeira de computação gráfica.

Existe um arquivo de configuração para a cena em /config/cena-config.txt

Arquivos .obj e .mtl devem ser adicionados na pasta /objects

Arquivos de textura devem ser adicionados na pasta /textures

## Configurações Globais

### Câmera

São definidos os valores de posição e inclinação da câmera de acordo com as variáveis: cameraPos, cameraFront e cameraUp. Cada uma recebe 3 coordenadas equivalentes à posição no eixo X, Y e Z respectivamente.

Exemplo:

```
cameraPos 0.0 0.0 3.0
cameraFront 0.0 0.0 -1.0
cameraUp 0.0 1.0 0.0
```

### Iluminação

São definidos os valores de posição da iluminação e cor da luz de acordo com as seguintes variáveis:

- lightPos: 3 coordenadas equivalentes à posição no eixo X, Y e Z
- lightColor: 3 coordenadas equivalentes às cores R, G e B

Exemplo:

```
lightPos -2.0 100.0 2.0
lightColor 1.0 1.0 1.0
```

## Configurações de OBJ

Podem ser adicionados multiplos objetos 3D na cena, seus parâmetros são configuráveis nesse arquivo.

- fileName: texto com nome do arquivo .obj
- position: 3 coordenadas equivalentes à posição no eixo X, Y e Z
- scale: valor em float que representa a escala
- angle: valor em float que representa o ângulo inicial de rotação
- axis: X, Y ou Z. Representa o eixo de rotação do obj

Exemplo:

```
fileName planeta.obj
position -1.0 0.0 0.0
scale 0.2
angle 0.0
axis Z
```

### Curvas

Ainda nas configurações do OBJ temos que definir as propriedades de curva.

- noCurve: indica que aquele objeto não possui trajetória
- startCurve: começa configuração dos pontos de controle
- curvePoint: adiciona ponto de controle com 3 coordenadas equivalentes à posição no eixo X, Y e Z
- endCurve: termina configuração dos pontos de controle

Exemplo sem curva:

```
noCurve
```

Exemplo com curva:

```
startCurve
curvePoint -0.7 -0.4 0.0
curvePoint -0.5 -0.4 1.0
curvePoint -0.2 -0.4 1.0
curvePoint -0.1 -0.4 0.5
curvePoint 0.0 -0.4 -1.0
curvePoint 0.2 -0.4 -1.0
curvePoint 0.4 -0.4 -1.0
curvePoint 0.6 -0.4 -1.0
curvePoint 0.7 -0.4 -0.5
curvePoint 0.8 1.0 1.0
endCurve
```

## Exemplo de arquivo final de configuração de cena

```
cameraPos 0.0 0.0 3.0
cameraFront 0.0 0.0 -1.0
cameraUp 0.0 1.0 0.0
lightPos -2.0 100.0 2.0
lightColor 1.0 1.0 1.0

fileName planeta.obj
position -1.0 0.0 0.0
scale 0.2
angle 0.0
axis Z
startCurve
curvePoint -0.7 -0.4 0.0
curvePoint -0.5 -0.4 1.0
curvePoint -0.2 -0.4 1.0
curvePoint -0.1 -0.4 0.5
curvePoint 0.0 -0.4 -1.0
curvePoint 0.2 -0.4 -1.0
curvePoint 0.4 -0.4 -1.0
curvePoint 0.6 -0.4 -1.0
curvePoint 0.7 -0.4 -0.5
curvePoint 0.8 1.0 1.0
endCurve

fileName SuzanneTriTextured.obj
position 0.0 0.9 0.0
scale 0.5
angle 0.0
axis Z
noCurve
```

## Interações na cena

A cena começa com o primeiro OBJ da lista selecionado. Todos os comandos serão aplicados individualmente apenas para o objeto selecionado.

### Comandos

- ENTER -> Seleciona próximo Objeto da lista
- \- e + -> Controla a escala
- 8 e 9 -> Controla o ângulo
- X -> Rotaciona no eixo X
- Y -> Rotaciona no eixo Y
- Z -> Rotaciona no eixo Z
- Setas para esquerda e direita -> Translação no eixo X
- Setas para cima e baixo -> Translação no eixo Y
- 4 e 5 -> Translação no eixo Z
- Movimentar mouse -> controla rotação da câmera
- WASD -> controla posição da câmera

OBS: Translação não funciona em objetos com trajetória, pois esses tem a sua posição redefinida pelos pontos de controle configurados previamente.
