# Let's Get The Aliens

UnB :: Tópicos Avançados em Computadores :: 2021/1

UnB :: Introdução ao Desenvolvimento de Jogos :: 2022/1

Gabriel Rocha Fontenele :: 15/1026760

## Como executar

Para executar em Windows, baixe a última versão do executável de 32-bits (compátivel com ambas as edições x86 e x64) disponível em [Releases](https://github.com/ngsylar/TACJE_IDJ/releases), extraia TODOS os arquivos da pasta zipada para um mesmo diretório e em seguida abra _GAME.exe_ (ou _GAME_with_log.exe_ para acompanhar o log de eventos).

Para executar em Linux, leia a seção [Como compilar](#como-compilar).

## Como compilar

Para compilar a partir da pasta raiz, tenha certeza de ter instalado o [SDL2](https://www.libsdl.org/download2.0.php) e as bibliotecas adicionais ([SDL2_IMG](https://www.libsdl.org/projects/SDL_image/), [SDL2_Mix](https://www.libsdl.org/projects/SDL_mixer/), [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)) e no terminal execute:
```bash
make        # para compilar
./GAME      # para executar
```

* No windows, inclua as dlls necessárias na pasta raiz (para abrir _GAME.exe_) ou em _extlib_ (para abrir _GAME_with_log.bat_) (veja _[extlib/requirements.txt](extlib/requirements.txt)_).

Para apagar os executáveis compilados, execute o arquivo _clear_obj.bat_.
