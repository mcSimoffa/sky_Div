### How to use Segger Embedded Studio for develop STM32 projects:

- Create a project in CubeMX and output the code for EWARM IAR. Most likely IAR should be installed, but you need to check
- Copy the EWARM folder next to the same under the name SES
- In SES-> Package manager, install the support package for the desired microcontroller (STM32F103 for example)
- Enter SES-> File-> Import {roject-> IAR EWARM-> Internal tollcahin (with other import options, the IAR limitation on the amount of code of the educational version 32Kb will be applied)
- Compile the project
- Remove *.eww, *ewd, *.ewp from the SES folder
- Pull into this folder: STM32F103xx_Registers.xml from c: \ Users \ boss \ AppData \ Local \ SEGGER \ SEGGER Embedded Studio \ v3 \ packages \ STM32F1xx \ XML \
- Connect the STM32F103xx_Registers.xml file in options-> Debugger-> register definition files
- Remove from project files: Cortex_M_Startup.s and add: STM32F1xx_Startup.s and STM32F103xx_Vectors.s. The last one is especially insidious: it compiles without it, but due to the absence of an interrupt vector table, when entering any interrupt, we fall into HardFault (which is described in Cortex_M_Startup.s)
- Can be compiled and debugged
The control transfer chain at startup: `` reset_handler '' in the STM32F1xx_Startup.s file, then `` _start '' to SEGGER_THUMB_Startup.s, and from there to `` PP_ENTRY_POINT```, which is the input to main ( ) 

### Как использовать Segger Embedded Studio для разработки STM32-проектов:
- Создать проект в CubeMX и вывести код для EWARM IAR. Скорее всего IAR должен быть установлен, но нужно проверить  
- Скопировать папку EWARM рядом же под именем SES  
- В SES->Package manager установить пакет поддержки нужного микроконтроллера(STM32F103 например)  
- Войти в SES->File->Import {roject->IAR EWARM->Internal tollcahin (при других вариантах импорта будет дейсвовать ограничение IAR на объем кода учебной версии 32Кб)  
- Скомпилировать проект  
- Удалить из папки SES *.eww, *ewd, *.ewp 
- Стянуть в эту папку: STM32F103xx_Registers.xml из c:\Users\boss\AppData\Local\SEGGER\SEGGER Embedded Studio\v3\packages\STM32F1xx\XML\  
- Подключить файл STM32F103xx_Registers.xml в options->Debugger->register definition files  
- Из файлов проекта удалить: Cortex_M_Startup.s и добавить: STM32F1xx_Startup.s и STM32F103xx_Vectors.s. Особенно коварный последний: без него компилируется, но ввиду отсутствия таблицы векторов прерывания при входе в любое прерывание падаем в HardFault (которое ксати описано в Cortex_M_Startup.s)  
- Можно компилировать и отлаживать  
Цепочка передачи управления при запуске: ```reset_handler``` в файле STM32F1xx_Startup.s, затем ```_start``` в SEGGER_THUMB_Startup.s, а оттуда в ```PP_ENTRY_POINT``` , что и есть вход в main()
