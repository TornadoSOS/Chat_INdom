const express = require('express');
const path = require('path');
const app = express();
const PORT = 8080;

// База данных 7 "Б" класса
const schoolData = {
    "7b": {
        monday: ["Физика 🔬", "Алгебра 📐", "Русский язык ✍️", "История 📜", "Физкультура 🏃‍♂️"],
        tuesday: ["Геометрия 📐", "Литература 📚", "Английский язык 🇬🇧", "Информатика 💻", "Биология 🌿"],
        wednesday: ["География 🌍", "Алгебра 📐", "Химия 🧪", "Технология 🛠️", "Музыка 🎵"]
    }
};

app.use(express.static(path.join(__dirname, 'public')));

app.get('/api/schedule/7b', (req, res) => {
    res.json(schoolData["7b"]);
});

app.listen(PORT, () => {
    console.log(`\n🚀 БОЛИД ЗАПУЩЕН!`);
    console.log(`🔗 Открой в браузере: http://localhost:${PORT}`);
});
