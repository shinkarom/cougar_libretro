const fs = require('fs-extra');
const archiver = require('archiver');
const path = require('path');
const tmp = require('tmp')
const browserify = require("browserify")
const execSync = require("child_process").execSync

const outputFileName = process.argv[2] || "output";

const outputFilePath = "./output/"+outputFileName+'.cart'; // Output .cart file name

(async () => {
  try {	
	const tmpobj = tmp.dirSync()
	const tempFolderPath =tmpobj.name
	
	if(!fs.exists("output"))
	await fs.mkdir("output")
    
	await fs.copy("tiles", path.join(tempFolderPath, "CHR"));
	
	await fs.mkdir(path.join(tempFolderPath, "SND"))
	
	const dir = await fs.promises.opendir("sounds")
	for await (const dirent of dir) {
		execSync("ffmpeg -i sounds/"+dirent.name+" -ac 1 -ar 16000 "+path.join(tempFolderPath, "SND", require("replace-ext")(dirent.name,".ogg")))
	}
	
	execSync("browserify -e src/index.js -s _MAIN -o "+ path.join(tempFolderPath, "PRG.js") +" -t [ babelify --presets [ @babel/preset-env ]] -t uglifyify")
		
	// Create a new archiver instance
    const archive = archiver('zip', {
      zlib: { level: 9 }, // Set compression level to maximum
    });
	
	// Pipe the archive data to the output file
    archive.pipe(fs.createWriteStream(outputFilePath));
    
	// Append the contents of the temporary folder to the archive
    archive.directory(tempFolderPath, false);
	
    // Finalize the archive and close the output file
    await archive.finalize();
    console.log(`"${outputFilePath}" created successfully.`);
  } catch (err) {
    console.error('Error:', err);
  }
})();