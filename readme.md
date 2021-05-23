Создать проект в CubeMX и вывести код для EWARM IAR. Скорее всего IAR должен быть установлен, но нужно проверить
Скопировать папку EWARM рядом же под именем SES
В SES->Package manager установить пакет поддержки нужного микроконтроллера(STM32F103 например)
Войти в SES->File->Import {roject->IAR EWARM->Internal tollcahin (при других вариантах импорта будет дейсвовать ограничение IAR на объем кода учебной версии 32Кб)
Скомпилировать проект
Удалить из папки SES все лишнее кроме файла emProject
Стянуть в эту папку: STM32F100C8_MemoryMap.xml и STM32F103xx_Registers.xml из c:\Users\boss\AppData\Local\SEGGER\SEGGER Embedded Studio\v3\packages\STM32F1xx\XML\
Подключить файл STM32F103xx_Registers.xml в options->Debugger->register definition files
Из файлов проекта удалить: Cortex_M_Startup.s и добавить: STM32F1xx_Startup.s и STM32F103xx_Vectors.s. Особенно коварный последний: без него компилируется, но ввиду отсутствия таблицы векторов прерывания при входе в любое прерывание падаем в HardFault (которое ксати описано в Cortex_M_Startup.s)
Можно компилировать и отлаживать 
Цепочка передачи управления при запуске: ```reset_handler``` в файле STM32F1xx_Startup.s, затем ```_start``` в SEGGER_THUMB_Startup.s, а оттуда в ```PP_ENTRY_POINT``` , что и есть вход в main()