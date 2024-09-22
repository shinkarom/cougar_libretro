const fs = require('fs-extra');
const archiver = require('archiver');
const path = require('path');
const tmp = require('tmp')
const browserify = require("browserify")
const execSync = require("child_process").execSync
const PNG = require('pngjs').PNG;
const getPixels = require('get-pixels');

const outputFileName = process.argv[2] || "output";

const outputFilePath = "./output/" + outputFileName + '.cart'; // Output .cart file name

(async () => {
    try {
        const tmpobj = tmp.dirSync()
        const tempFolderPath = tmpobj.name

        if (!fs.exists("output"))
            await fs.mkdir("output")

        //++++Images

        const globalPalette = new Set(); // Initialize the global color palette

        // Function to validate a PNG file
        function validatePNG(filePath) {
            return new Promise((resolve, reject) => {
                const fileStream = fs.createReadStream(filePath);
                const png = new PNG();

                fileStream.pipe(png)
                    .on('parsed', function() {
                        const width = this.width;
                        const height = this.height;

                        if (width !== 128 || height !== 128) {
                            reject(new Error(`File ${filePath} must be 128x128 pixels.`));
                            return;
                        }
                    })
                    .on('error', (err) => {
                        reject(err);
                    });
            });
        }

		fs.mkdirSync(path.join(tempFolderPath, "CHR"))
        const files = await fs.promises.readdir("tiles");

        const validFiles = [];

        for (const file of files) {
            if (/^CHR_[0-9A-Fa-f]{2}\.png$/.test(file)) {
                const filePath = path.join("tiles", file);
                try {
                    const validFile = await validatePNG(filePath, tileColorLimit, fileColorLimit, globalColorLimit);
                    const destinationPath = path.join(tempFolderPath, file);

                    fs.promises.copyFile(filePath, path.join(tempFolderPath, "CHR", file));
                    //console.log(`Valid file copied to: ${destinationPath}`);
                    validFiles.push(file);
                } catch (error) {
                    throw new Error(`Error processing ${filePath}: ${error.message}`);
                }
            }
            // Check the global palette against the global color limit
            if (globalColorLimit > 0 && globalPalette.size > globalColorLimit) {
                throw new Error(`Global color limit exceeded: ${globalPalette.size} colors, limit is ${globalColorLimit}.`);
            }

        }
        //----images
        await fs.mkdir(path.join(tempFolderPath, "SND"))

        const dir = await fs.promises.opendir("sounds")
        for await (const dirent of dir) {
            var convertLine = "ffmpeg -i sounds/" + dirent.name + " -ac 1 -ar 16000 " + path.join(tempFolderPath, "SND", require("replace-ext")(dirent.name, ".ogg"))
            console.log(convertLine)
            execSync(convertLine)
        }

        execSync("browserify -e src/index.js -s _MAIN -o " + path.join(tempFolderPath, "PRG.js") + " -t [ babelify --presets [ @babel/preset-env ]] -t uglifyify")

        // Create a new archiver instance
        const archive = archiver('zip', {
            zlib: {
                level: 9
            }, // Set compression level to maximum
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