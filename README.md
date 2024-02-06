# Texture-Builder

Application that packages images and metadata into a .bin file

I built this for myself. I am not a great programmer, so use at your own risk. Or don't, I'm not your boss.

I built this to use with my own game engine.

All it does it take image files from the 'textures' folder, and packages them into 'textures.bin' along with any relevant metadata (mainly height, width, channels, offset), which I can then copy to my game engine logic

My game engine then has logic so that any time there's a call for a texture, it knows to look for the 'textures.bin', and retrieve the image by name (extentions are removed on compile, so "Test.png" becomes "Test").

Note if you use this, your game engine will need to have logic to read the data.

User the "Compiler.bat" to compile the code

User "TexturePackager.exe" to take all textures in the /textures folder, and export them to textures.bin

For troubleshooting, you can also use "TextureReader.exe", which will look for textures.bin in the same directory, and output TextureReader.log which'll confirm the data written: image name, size, channels, offset, etc.

The reason for the last step is I had a lot of issues getting OpenGL to read it right and needed to confirm whether it was the reading or the writing that was the issue. The Reader has confirmed it writes fine, so is largely redundant now that's confirmed, but I've left it in just in case ever needed for troubleshooting again

I may expand on this in the future, but for now it's uploaded as-is. Happy to collaborate to expand on if needed, although my intention was always just to have it as a quick and dirty program to get the stuff I needed into my main project as fast as possible, hence why the code is messy, but for actually using it requires minimal steps other than dumping your textures in /textures.
