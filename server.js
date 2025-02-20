const express = require("express");
const multer = require("multer");
const { exec } = require("child_process");
const fs = require("fs");
const path = require("path");

const app = express();
const upload = multer({ dest: "uploads/" });

app.post("/analyze", upload.single("file"), (req, res) => {
    if (!req.file) {
        return res.status(400).json({ error: "No file uploaded." });
    }
    
    const filePath = path.join(__dirname, req.file.path);
    const analyzerPath = path.join(__dirname, "analyzer"); // C++ executable

    exec(`${analyzerPath} ${filePath}`, (error, stdout) => {
        fs.unlinkSync(filePath); // Delete file after processing
        if (error) {
            return res.status(500).json({ error: "Analysis failed." });
        }
        res.json(JSON.parse(stdout));
    });
});

app.listen(5000, () => console.log("Server running on port 5000"));
