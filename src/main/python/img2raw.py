from PIL import Image

image = Image.open("resources/32x64-image.png")
pixels = image.load()

out_file = open("target/image.bin", "wb")

for y in range(64):
    for x in range(32):
        out_file.write(chr(pixels[x,y]))
