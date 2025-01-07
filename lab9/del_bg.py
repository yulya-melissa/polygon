# Удаление фона с картинки

from PIL import Image
import numpy as np
import rembg

image = Image.open('cats.jpg')
data = np.array(image)
# data[np.logical_and(data[:, :, 0] > 163, data[:, :, 0] < 229) &
#         np.logical_and(data[:, :, 1] > 121, data[:, :, 1] < 185) &
#         (data[:, :, 2] < 57), :] = 0
out = rembg.remove(data)
res = Image.fromarray(out)
res.save('new.png')