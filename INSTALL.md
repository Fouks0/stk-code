## Building from source

In order to build SuperTuxKart Addons, look [here](https://github.com/supertuxkart/stk-code/blob/master/INSTALL.md).

Changes:

  * Use `git clone https://github.com/Fouks0/stk-code.git -b Addons` (also ensure that the code matches the latest Addons release, else use `git branch` with an existing tag to have the good code version)
  * You need to download the assets from the corresponding Addons Release [here](https://github.com/Fouks0/stk-code/releases) (it is a `data` folder)
  * You must put that `data` folder in the stk-code directory
  * You must not use any other asset folder (in particular, not have a `stk-assets` next to `stk-code`)
