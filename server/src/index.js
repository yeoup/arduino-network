"use strict";

const fs = require("fs");
const path = require('path');
const { nanoid } = require("nanoid"); // 랜덤 문자열 생성기
const router = require("express").Router();
const { imageDirectory } = require("../filePath") // 파일 저장 경로

router.post("/arduino", (req, res) => {
    const gas = req.body.gas; // 가스 센서값
    const base64Data = req.body.image;
    const fileName = String(Date.now()) + nanoid() + ".jpg"; // 파일 이름 '현재 시간 + 랜덤문자열 + 파일 확장자(jpg)'
    const filePath = path.join(imageDirectory, 'images', fileName); // 저장 경로 'images'
    fs.writeFile(filePath, base64Data, 'base64', err => { // Base64 이미지 데이터를 파일로 디코딩하여 저장
        if (err) {
            console.error('파일 저장 오류:', err);
            res.sendStatus(500);
        }
    });
    console.log("파일 접근 경로: [Server IP]/images/" + fileName);
    res.send(req.body);
})

module.exports = router;