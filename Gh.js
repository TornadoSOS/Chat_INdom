// Функция для загрузки расписания конкретного дня
async function loadDay(dayName) {
    // Меняем активную кнопку в меню
    const buttons = document.querySelectorAll('.day-btn');
    buttons.forEach(btn => btn.classList.remove('active'));
    event.target.classList.add('active');

    const container = document.getElementById('schedule-container');
    container.innerHTML = ''; // Очищаем старые уроки

    try {
        // Делаем асинхронный запрос к нашему Node.js API
        const response = await fetch('/api/schedule/7b');
        const data = await response.json();
        
        const lessons = data[dayName];

        // Рендерим каждый урок с каскадной задержкой анимации
        lessons.forEach((lesson, index) => {
            const card = document.createElement('div');
            card.className = 'lesson-card';
            
            // Задержка анимации (animation-delay), чтобы каждый следующий урок вылетал чуть позже предыдущего
            card.style.animationDelay = `${index * 0.08}s`; 
            
            card.innerHTML = `
                <div class="lesson-num">${index + 1}</div>
                <div class="lesson-name">${lesson}</div>
            `;
            container.appendChild(card);
        });

    } catch (error) {
        container.innerHTML = `<p style="color:#ef4444; text-align:center;">Ошибка загрузки данных</p>`;
    }
}

// Загружаем понедельник при первом открытии сайта
document.addEventListener('DOMContentLoaded', () => {
    // Имитируем клик по первой кнопке
    document.querySelector('.day-btn').click();
});
