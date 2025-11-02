# Изменения в сборке Emscripten для libktx

## Обзор

Этот документ описывает изменения, внесенные для улучшения сборки libktx с Emscripten, включая добавление helper функций для прямого доступа через cwrap API и оптимизацию размера сборки.

## Ключевые изменения

### 1. Новые helper функции (lib/src/ktx_emscripten_helpers.c)

Добавлены C функции с EMSCRIPTEN_KEEPALIVE для прямого доступа к членам структуры ktxTexture2:

```c
// Доступ к свойствам текстуры
uint8_t* ktx_get_data(ktxTexture2* texture);
size_t ktx_get_data_size(ktxTexture2* texture);
uint32_t ktx_get_base_width(ktxTexture2* texture);
uint32_t ktx_get_base_height(ktxTexture2* texture);
uint32_t ktx_get_base_depth(ktxTexture2* texture);
uint32_t ktx_get_num_levels(ktxTexture2* texture);
uint32_t ktx_get_num_layers(ktxTexture2* texture);
uint32_t ktx_get_num_faces(ktxTexture2* texture);
uint32_t ktx_get_num_dimensions(ktxTexture2* texture);
uint32_t ktx_get_is_array(ktxTexture2* texture);
uint32_t ktx_get_is_cubemap(ktxTexture2* texture);
uint32_t ktx_get_is_compressed(ktxTexture2* texture);
uint32_t ktx_get_vk_format(ktxTexture2* texture);
uint32_t ktx_get_supercompression_scheme(ktxTexture2* texture);
ktx_error_code_e ktx_get_image_offset(ktxTexture2*, uint32_t, uint32_t, uint32_t, ktx_size_t*);
ktx_size_t ktx_get_image_size(ktxTexture2* texture, uint32_t level);
```

**Почему это необходимо:**
- Emscripten/WASM не может напрямую обращаться к членам C структур из JavaScript
- embind API добавляет накладные расходы и увеличивает размер кода
- cwrap API более легковесен и дает больше контроля

### 2. Обновленные флаги линкера в CMakeLists.txt

#### В CMakeLists.txt (корень проекта):

Добавлены следующие флаги в `KTX_EM_COMMON_LINK_FLAGS`:

```cmake
"SHELL:-s ASSERTIONS=0"  # Отключить assertions для уменьшения размера
"SHELL:-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','getValue','setValue','UTF8ToString','HEAPU8','GL','HEAP8']"
"SHELL:-s EXPORTED_FUNCTIONS=['_malloc','_free','_ktxTexture2_CreateFromMemory', ...]"
```

**Экспортируемые функции:**
- Стандартные: `_malloc`, `_free`, `_ktxErrorString`
- libktx: `_ktxTexture2_CreateFromMemory`, `_ktxTexture2_Destroy`, `_ktxTexture2_TranscodeBasis`, `_ktxTexture2_NeedsTranscoding`
- Helper: `_ktx_get_*` (все новые helper функции)

#### В lib/CMakeLists.txt:

Добавлено условное включение helper файла:

```cmake
if(EMSCRIPTEN)
    list(APPEND LIBKTX_MAIN_SRC
        src/ktx_emscripten_helpers.c
    )
endif()
```

### 3. Оптимизация размера сборки

Уже присутствуют (подтверждены):
- `-s FILESYSTEM=0` - отключена файловая система (~500 KB экономии)
- `-s NO_EXIT_RUNTIME=1` - runtime не завершается после main()
- `-s ALLOW_MEMORY_GROWTH=1` - динамическое увеличение памяти

Добавлено:
- `-s ASSERTIONS=0` - отключены runtime проверки

## Использование

### Вариант 1: embind API (существующий)

```javascript
const ktx = await createKtxModule();
const texture = new ktx.texture(ktx2Data);
console.log(texture.baseWidth, texture.baseHeight);
```

### Вариант 2: cwrap API (новый)

```javascript
const M = await createKtxModule();
const api = {
    malloc: M.cwrap('malloc', 'number', ['number']),
    free: M.cwrap('free', null, ['number']),
    getBaseWidth: M.cwrap('ktx_get_base_width', 'number', ['number']),
    // ... и т.д.
};

// Использование
const dataPtr = api.malloc(ktx2Data.byteLength);
M.HEAP8.set(ktx2Data, dataPtr);
// ... работа с текстурой через указатели
```

## Сборка

### Из Docker (рекомендуется):

```bash
./scripts/build_wasm_docker.sh
```

### Локально с Emscripten:

```bash
source /path/to/emsdk/emsdk_env.sh
emcmake cmake -B buildwasm -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DKTX_FEATURE_TOOLS=OFF \
    -DKTX_FEATURE_TESTS=OFF
cmake --build buildwasm
```

## Ожидаемый размер файлов

После оптимизации:

- `libktx.mjs`: ~200-250 KB
- `libktx.wasm`: ~1.5-2.0 MB (с ASSERTIONS=0 и FILESYSTEM=0)
- `libktx_read.mjs`: ~50 KB
- `libktx_read.wasm`: ~200-300 KB

**С gzip сжатием:**
- Полная сборка: ~500-700 KB
- Read-only сборка: ~70-100 KB

## Проверка экспортированных функций

После сборки проверьте доступность функций:

```javascript
const M = await createKtxModule();

console.log('cwrap:', typeof M.cwrap);  // должно быть 'function'
console.log('_malloc:', typeof M._malloc);  // должно быть 'function'
console.log('_ktx_get_data:', typeof M._ktx_get_data);  // должно быть 'function'
```

## Дополнительная документация

- `EMSCRIPTEN_USAGE_RU.md` - подробные примеры использования с cwrap и embind API
- `interface/js_binding/ktx_wrapper.cpp` - embind bindings
- `lib/src/ktx_emscripten_helpers.c` - helper функции для cwrap

## Известные проблемы

1. При использовании cwrap API необходимо вручную управлять памятью (malloc/free)
2. Указатели передаются как числа (number), требуется осторожность с типами
3. Строки требуют преобразования через UTF8ToString

## TODO

- [ ] Добавить TypeScript типы для cwrap API
- [ ] Создать высокоуровневую обёртку над cwrap API
- [ ] Добавить unit тесты для helper функций
- [ ] Документировать все VkFormat значения
