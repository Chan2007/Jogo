

Este é um projeto desenvolvido em C++ e em Windows que integra a interface gráfica do **Qt 5** (para menus, configurações e telas de interface) com a biblioteca de jogos **SFML 2.6.0** (para o motor do jogo, renderização e áudio).

O projeto foi estruturado utilizando padrões estritos do **C++03 / C++98** e com  a biblioteca SFML, facilitando o compartilhamento e execução em diferentes ambientes de desenvolvimento (IDEs) sem a necessidade de reconfigurações complexas de caminhos no sistema.

---

## Aviso Legal e Licença
Este é um projeto acadêmico e sem fins lucrativos. Para detalhes sobre o uso de ativos de terceiros (Riot Games, Itch.io, etc.) e termos de isenção de responsabilidade, consulte os arquivos [LICENSE.txt](./LICENSE.txt) e [DISCLAIMER.txt](./DISCLAIMER.txt).

## Pré-requisitos Obrigatórios

Certificar-se de ter instalado:
1. **Compilador MSVC (Microsoft Visual Studio 2019 ou superior):** Com *"Desenvolvimento para desktop com C++"* ativada.
2. **Qt 5.15 (MSVC 2019 64-bit):** Instalado no diretório padrão (ex: `C:\\Qt\\5.15.19\\msvc2019_64`).

*Nota: O SFML 2.6.0 (versão de 64 bits para MSVC) já está incluso na raiz do projeto dentro da pasta `/SFML-2.6.0`, portanto **não** é necessário baixá-lo ou instalá-lo manualmente.*

---

## Execução
1. Clone o Repositório.
2. Dependendo do IDE:
- ### ***Qt Creator*** 
    O Qt Creator utiliza o arquivo de configuração ```.pro``` para mapear o projeto de forma direta.
  - Abra o **Qt Creator**.
  - Clique em **Open Project** (Abrir Projeto) e selecione o arquivo ```GUI_Jogo.pro``` localizado na raiz da pasta do projeto.

  - Na tela de configuração de Kits, selecione obrigatoriamente o kit correspondente ao compilador: ***Desktop Qt 5.15.x MSVC2019 64bit***

  - Clique em **Configure Project**.

  - No menu lateral esquerdo, clique com o botão direito sobre o nome do projeto e selecione **Run qmake**.

  - Clicar no botão de "**Run**" (verde) para compilar e rodar o jogo.

- ### ***Visual Studio*** 
  O Visual Studio consegue abrir projetos baseados em CMake nativamente a partir das versões mais recentes.
    - Abra o **Visual Studio**.
  
    - Na tela inicial, selecione a opção **Open a local folder** (Abrir uma pasta local).

    - Selecione a pasta raiz do projeto (onde está localizado o arquivo *CMake*).

    - O Visual Studio irá detectar o arquivo do CMake e começará a gerar o cache automaticamente no menu inferior.

    - Na barra de ferramentas superior, localize o menu de seleção de alvos (geralmente exibe ```x64-Debug``` ou "**Selecione o Item de Inicialização**").

    - Altere a seleção para ```Jogo.exe``` (subdestino) ou o nome correspondente ao executável do projeto.

    - Pressione **F5** ou clique no botão de *Play/Iniciar* para compilar e rodar.

- ### ***CLion*** 
  O CLion gerencia o projeto utilizando o arquivo ```CMakeLists.txt```. Como o CLion roda os executáveis de forma isolada, precisamos garantir que o compilador correto seja selecionado.
    1. #### Configurar o Compilador (Toolchain)
        - No CLion, vá para ***File > Settings > Build, Execution, Deployment > Toolchains***.
        - Clique no ícone de "*+*" para adicionar um novo toolchain.
        - Garanta que o CLion detectou a pasta do Visual Studio e selecione o compilador do Visual Studio (*MSVC*) correspondente à versão instalada (ex: Visual Studio 2019).
        - Mude o campo Architecture para **amd64** ou **x64**.
        - Certifique-se de que o CMake e o Debugger estejam configurados corretamente para usar o *MSVC*.
        - Mova essa Toolchain do Visual Studio para o topo da lista (deixando-a como *Default*). Clique em **Apply**.
       
    2. #### Configurar o Perfil do CMake
       - Na janela de configurações, clique na opção *CMake* (logo abaixo de Toolchains).
       - No seu perfil ativo (geralmente *Debug* ou *RelWithDebInfo*), altere a caixinha Toolchain de MinGW para Visual Studio, se ainda não o estar feito.
       - Clique em OK para salvar e fechar as configurações.
       
    3. #### Limpar o Cache e Executar
       - No menu superior do CLion, vá em ***Tools > CMake > Reset Cache and Reload Project***.
       - No menu superior, vá em ***Build > Rebuild 'Jogo'*** (isso fará o *CMake* a copiar as DLLs necessárias para a pasta do executável).
       - Clique no botão de "*Run*" Verde no topo da tela para iniciar o jogo.

## Integração Contínua (CI)

Este projeto utiliza *GitHub Actions* para:

- Compilar automaticamente em Windows.
- Gerar builds nas configurações **Debug** e **Release**.
- Disponibilizar os executáveis como artefatos para download após cada compilação bem-sucedida.

- ### Testes de Segurança
[![CodeScene Average Code Health](https://codescene.io/projects/81933/status-badges/average-code-health)](https://codescene.io/projects/81933)
[![CodeScene Hotspot Code Health](https://codescene.io/projects/81933/status-badges/hotspot-code-health)](https://codescene.io/projects/81933)
[![CodeScene System Mastery](https://codescene.io/projects/81933/status-badges/system-mastery)](https://codescene.io/projects/81933)
[![CodeScene general](https://codescene.io/images/analyzed-by-codescene-badge.svg)](https://codescene.io/projects/81933)

## Star History

<a href="https://www.star-history.com/?repos=Chan2007%2FJogo.git&type=date&legend=top-left">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/chart?repos=Chan2007/Jogo.git&type=date&theme=dark&legend=top-left&sealed_token=UpIKj-r32UEKI39mHPy9ljx2eZPdn4mnojvgM0m2f0XrzfKm2x5Ar92Sj73cBJDbjNmu6VkmcUTYojWprR6331XkJmHRz-y90AhpJ6H9psZiqslEavfnPIz6qd7adUqn9SOUhHneo4Rd_D-uKN6pMbH0PjEx38d4kkLIm2Vuk3pCHkG4dgmmAp7wZ2ap" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/chart?repos=Chan2007/Jogo.git&type=date&legend=top-left&sealed_token=UpIKj-r32UEKI39mHPy9ljx2eZPdn4mnojvgM0m2f0XrzfKm2x5Ar92Sj73cBJDbjNmu6VkmcUTYojWprR6331XkJmHRz-y90AhpJ6H9psZiqslEavfnPIz6qd7adUqn9SOUhHneo4Rd_D-uKN6pMbH0PjEx38d4kkLIm2Vuk3pCHkG4dgmmAp7wZ2ap" />
   <img alt="Star History Chart" src="https://api.star-history.com/chart?repos=Chan2007/Jogo.git&type=date&legend=top-left&sealed_token=UpIKj-r32UEKI39mHPy9ljx2eZPdn4mnojvgM0m2f0XrzfKm2x5Ar92Sj73cBJDbjNmu6VkmcUTYojWprR6331XkJmHRz-y90AhpJ6H9psZiqslEavfnPIz6qd7adUqn9SOUhHneo4Rd_D-uKN6pMbH0PjEx38d4kkLIm2Vuk3pCHkG4dgmmAp7wZ2ap" />
 </picture>
</a>

[![Build](https://github.com/Chan2007/Jogo/actions/workflows/actions.yml/badge.svg?branch=teste-github-actions)](.github/workflows/build.yml)

<!--
Música de fundo:

- Life in Ruins (The Legend of Zelda: Breath of the Wild)
- OMORI OST
- Menu theme, End credits (Life is Strange)
- Ori and the Will of the Wisps OST
- Lost voice, Lumière à l’Aube, Lune, Lumière (Music Box) (Clair Obscur: Expedition 33)
- Neeko, Nami login screen (League of Legends)
- Reflection, Queen's Gardens, The White Lady (Hollow Knight)
- City Ruins - Rays of Light (NieR:Automata)
- Dawn Winery, Happy Journey, Twilight Serenity, Dusk in Mondstadt (Genshin Impact)
- Miquella's Haligtree (Elden Ring)
- Ezio's Family (Assassin's Creed II)

-->



<!--
- Character Creation (Elden Ring)
- The Only Thing They Fear Is You (DOOM Eternal)
- Stratospheric, Champion Select Music, Level Up (Worlds 2024) (League of Legends)
- Bitten Bullet (MSI 2022) (League of Legends)
- Champion Select (MSI 2019) (League of Legends)
- Champion Select (Worlds 2018) (League of Legends)
- Aerolite - Alec Justice (LCK 2024) (League of Legends)
- Champion Select QF T1 vs AL (Worlds 2025) (League of Legends)

-->

<!--
Chefão:

- Freljord | Music (League of Legends)
- Dragonborn (The Elder Scrolls V: Skyrim)
- Menu Theme (Full) (Fortnite)
- On The Run (Detroit: Become Human)
- Battling Breeze (Clair Obscur: Expedition 33)
- God of War (God of War)
- Bring Home The Glory (instrumental) (League of Legends)
- Lulu, Draven, Jayce, Syndra login theme (League of Legends)
- Summoner's Call, Champion Select - Draft Pick (League of Legends)
- Child of Light OST
- Wolds Orchestral OST (League of Legends)
- Amusement Part (NieR:Automata)
- Caelestinum Finale Termini, Rite of Battle (Genshin Impact)
- My Rules (Worlds 2020) (League of Legends)
- Champion Select (LCK 2024) (League of Legends)
- Worlds 2016 Finals (Login Screen) (League of Legends)
- Dimensions Collide - Chris Brett (League of Legends)
- Season 2016 (Login Screen) (League of Legends)
- Main Theme (Batman Arkham City)
- Main Theme (Assassin's Creed Origins)
- Unity - Chris Tilton (Assissin's Creed Unity)
- Main Theme (Assassin's Creed IV: Black Flag)
- Main Theme (Assassin's Creed III)
-->
