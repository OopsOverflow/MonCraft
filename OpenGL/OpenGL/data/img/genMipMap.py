#!/usr/bin/env python

# Hello World in GIMP Python

from gimpfu import *

def hello_world(img, drawable, tileCountX, tileCountY, level):

    pdb.gimp_undo_push_group_start(img)
    pdb.gimp_context_set_interpolation(INTERPOLATION_LINEAR)

    tileCountX = int(tileCountX)
    tileCountY = int(tileCountY)
    level = int(level)
    tileSize = img.width / tileCountX


    factor = pow(2, level)

    for i in range(tileCountX):
        for j in range(tileCountX):

            offX = i * tileSize
            offY = j * tileSize

            pdb.gimp_image_select_rectangle(img, CHANNEL_OP_REPLACE, offX, offY, tileSize, tileSize)
            drawable = pdb.gimp_image_get_active_drawable(img)
            sel = pdb.gimp_item_transform_scale(drawable, offX/factor, offY/factor, (offX+tileSize)/factor, (offY+tileSize)/factor)
            pdb.gimp_floating_sel_anchor(sel)

    img.resize(img.width/factor, img.height/factor, 0, 0)

    pdb.gimp_undo_push_group_end(img)


register(
    "python_fu_moncraft_mipmap",
    "Gen MipMap for MonCraft",
    "Generates mipmap levels for a texture atlas",
    "Mathis Brossier",
    "Mathis Brossier",
    "2021",
    "<Image>/File/MIPMAP",
    "",
    [
        (PF_SPINNER, "tileCountX", "Tile count (X)", 8, (1, 100, 1)),
        (PF_SPINNER, "tileCountY", "Tile count (Y)", 8, (1, 100, 1)),
        (PF_SPINNER, "level", "Mipmap level", 1, (1, 10, 1)),
    ],
    [],
    hello_world)

main()
