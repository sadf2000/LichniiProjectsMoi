// Здесь вайбкодил, но с помощью массивов сам делал музыку
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    float freq;
    float duration;
    float volume; // 0.0 to 1.0
} Note;
#define C4  261.63f
#define D4  293.66f
#define E4  329.63f
#define F4  349.23f
#define G4  392.00f
#define A4  440.00f
#define B4  493.88f
#define C5  523.25f
#define D5  587.33f
#define E5  659.26f
#define F5  698.46f
#define G5  783.99f
// Улучшенная функция генерации WAV с поддержкой нескольких нот
void generate_melody_wav(const char* filename, Note* notes, int note_count) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;

    // Параметры аудио
    const uint32_t sample_rate = 44100;
    const uint16_t channels = 1;
    const uint16_t bits_per_sample = 16;
    
    // Рассчитываем общую длину данных
    float total_duration = 0;
    for (int i = 0; i < note_count; i++) {
        total_duration += notes[i].duration;
    }
    uint32_t data_size = (uint32_t)(total_duration * sample_rate) * sizeof(int16_t);

    // Записываем заголовок WAV
    fwrite("RIFF", 1, 4, f);
    uint32_t chunk_size = 36 + data_size;
    fwrite(&chunk_size, 4, 1, f);
    fwrite("WAVEfmt ", 1, 8, f);
    uint32_t fmt_size = 16;
    fwrite(&fmt_size, 4, 1, f);
    uint16_t audio_format = 1; // PCM
    fwrite(&audio_format, 2, 1, f);
    fwrite(&channels, 2, 1, f);
    fwrite(&sample_rate, 4, 1, f);
    uint32_t byte_rate = sample_rate * channels * bits_per_sample / 8;
    fwrite(&byte_rate, 4, 1, f);
    uint16_t block_align = channels * bits_per_sample / 8;
    fwrite(&block_align, 2, 1, f);
    fwrite(&bits_per_sample, 2, 1, f);
    fwrite("data", 1, 4, f);
    fwrite(&data_size, 4, 1, f);

    // Генерация звука для каждой ноты
    for (int n = 0; n < note_count; n++) {
        Note note = notes[n];
        uint32_t samples = note.duration * sample_rate;
        
        for (uint32_t i = 0; i < samples; i++) {
            float t = (float)i / sample_rate;
            
            // Основной тон
            float sample = sin(2 * M_PI * note.freq * t);
            
            // Простая огибающая для уменьшения кликов
            float envelope = 1.0;
            if (i < 100) envelope = (float)i / 100; // Атака
            if (i > samples - 100) envelope = (float)(samples - i) / 100; // Затухание
            
            // Применяем громкость и огибающую
            sample *= note.volume * envelope * 32767.0f;
            
            // Записываем сэмпл
            int16_t output = (int16_t)sample;
            fwrite(&output, 2, 1, f);
        }
    }

    fclose(f);
}

// Простая мелодия (первые ноты "Happy Birthday")
int main() {
    /*Note melody[] = {
        // Нота, длительность (сек), громкость (0-1)
        {440.00f, 0.3f, 0.8f}, // A4
        {440.00f, 0.3f, 0.8f}, // A4
        {493.88f, 0.6f, 0.8f}, // B4
        {440.00f, 0.6f, 0.8f}, // A4
        {523.25f, 0.6f, 0.8f}, // C5
        {493.88f, 1.0f, 0.8f}, // B4
        
        {440.00f, 0.3f, 0.8f}, // A4
        {440.00f, 0.3f, 0.8f}, // A4
        {493.88f, 0.6f, 0.8f}, // B4
        {440.00f, 0.6f, 0.8f}, // A4
        {587.33f, 0.6f, 0.8f}, // D5
        {523.25f, 1.0f, 0.8f}, // C5
    };*/
    Note melody[] = {
        // Нота, длительность (сек), громкость (0-1)
        {D4, 1.0f, 0.7f}, {F4, 0.5f, 0.7f}, {G4, 0.5f, 0.7f},
        {D5, 0.5f, 0.7f}, {C5, 0.5f, 0.7f}, {A4, 0.5f, 0.7f},
        {G4, 0.5f, 0.7f}, {A4, 0.5f, 0.7f}, {C5, 0.5f, 0.7f},
        {D5, 1.0f, 0.7f}, {F5, 0.5f, 0.7f}, {E5, 0.5f, 0.7f},
        {D5, 0.5f, 0.7f}, {C5, 0.5f, 0.7f}, {A4, 0.5f, 0.7f},
        {G4, 1.0f, 0.7f}, {G4, 0.5f, 0.7f}, {A4, 0.5f, 0.7f},
        {C5, 0.5f, 0.7f}, {D5, 1.0f, 0.7f}, {F4, 0.5f, 0.7f},
        {G4, 1.5f, 0.7f}
    };
    generate_melody_wav("melody.wav", melody, sizeof(melody)/sizeof(Note));
    
    return 0;
}
