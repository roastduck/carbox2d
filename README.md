# Carbox2d

симулятор эволюции машинок

Идея взята с сайта http://boxcar2d.com

![Carbox2d](https://raw.github.com/macrosoft/carbox2d/master/img/carbox2d.png)

## BUILD / СБОРКА

1. Make sure Box2D (suggesting 2.3.0) is in your path.

2. Run `qmake` (Qt 5 required).

3. Run `make`.

4. The output executable is `carbox2d`.

-------------

1. Скачайте и распакуйте физический движок с сайта http://box2d.org/. В данной версии carbox2d используется box2d 2.3.0

2. Соберите движок box2d по инструкции в файле Building.txt

3. Создайте в каталоге с проектом каталог lib и скопируйте в него полученный файл $BOX2DPATH/Build/Box2D/libBox2D.a

4. Скопируйте в каталог с проектом директорию Box2D, содержащую заголовочные файлы от физического движка

5. Теперь проект можно открыть и собрать в Qt Creator
