"use strict";

const fs = require("fs");
const app = require("../app");
const PORT = process.env.PORT || 3000; // 포트

// 서버 가동
app.listen(PORT, async () => {
  const dir = "./images"; // 업로드된 파일을 저장할 경로
    if (!fs.existsSync(dir)) {
        fs.mkdirSync(dir); 
    }
    console.log(`Server running on port ${PORT}`);
});
