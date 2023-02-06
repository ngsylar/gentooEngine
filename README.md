# Gentoo Engine

UnB :: Introdução ao Desenvolvimento de Jogos :: 2022/2

Gabriel Rocha Fontenele :: 15/1026760
Moisés Felipe Jacó Andrade de Lima :: 19/0018364

## Como compilar

Para compilar a partir da pasta raiz, tenha certeza de ter instalado o [SDL2](https://www.libsdl.org/download2.0.php) e as bibliotecas adicionais ([SDL2_IMG](https://www.libsdl.org/projects/SDL_image/), [SDL2_Mix](https://www.libsdl.org/projects/SDL_mixer/), [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)) e no terminal execute:
```bash
make        # para compilar
./GAME      # para executar
```

* No windows, inclua as dlls necessárias na pasta raiz (para abrir _GAME.exe_) ou em _extlib_ (para abrir _GAME_with_log.bat_) (veja _[extlib/requirements.txt](extlib/requirements.txt)_).

Para apagar os executáveis compilados, execute o arquivo _clear_obj.bat_.
