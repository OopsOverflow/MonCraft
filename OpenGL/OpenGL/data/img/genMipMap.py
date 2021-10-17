import numpy as np
import cv2
import argparse
from pathlib import Path
import math


def isPow2(n):
    return (n & (n-1) == 0) and n != 0


parser = argparse.ArgumentParser(description='Generate mipmap for MonCraft.')
parser.add_argument('atlas', help='original texture atlas')
parser.add_argument('tileSize', type=int, help='tileSize of a tile in the atlas')
args = parser.parse_args()

atlasPath = Path(args.atlas)
tileSize = args.tileSize
levels = int(math.log2(tileSize))

if not atlasPath.exists():
    raise f"the atlas file does not exist: '{str(atlasPath)}'"

if not isPow2(tileSize):
    raise 'tileSize must be a power of 2'

img = cv2.imread(args.atlas, cv2.IMREAD_UNCHANGED)
imgSize = img.shape[0]

if img.shape[0] != img.shape[1]:
    raise 'image must be square'

if imgSize % tileSize != 0:
    raise "image size and tile size are incompatible"

tileCount = int(imgSize / tileSize)

newSize = tileSize

for i in range(levels):
    outPath = atlasPath.parent / (atlasPath.stem + str(i + 1) + atlasPath.suffix)
    newSize = int(newSize / 2)
    newImg = np.copy(img)[:tileCount * newSize, :tileCount * newSize]

    for row in range(tileCount):
        for col in range(tileCount):
            oldTile = img[row * tileSize:(row+1) * tileSize, col * tileSize:(col+1) * tileSize]
            newTile = newImg[row * newSize:(row+1) * newSize, col * newSize:(col+1) * newSize]
            newTile[:] = cv2.resize(oldTile, (newSize, newSize), cv2.INTER_LINEAR)

    cv2.imwrite(str(outPath), newImg)
