# Bezier Surface

OpenGL ve GLUT ile yazılmış etkileşimli Bezier yüzeyi uygulaması.

## Yapılan düzeltmeler

- Eksik `mouse.h` bağımlılığı kaldırıldı.
- 16 kontrol noktası tek tek değişkenler yerine dizi yapısına taşındı.
- Tekrarlı nokta seçme ve güncelleme kodu döngü tabanlı hale getirildi.
- Yüzey geometri ve normalleri her karede değil, sadece kontrol noktası değiştiğinde yeniden hesaplanacak şekilde önbelleklendi.
- Tek tampon yerine çift tampon kullanıldı; çizim daha akıcı hale geldi.
- Linux üzerinde doğrudan derlenebilmesi için `Makefile` eklendi.

## Gereksinimler

Ubuntu/Debian:

```bash
sudo apt-get install freeglut3-dev g++
```

## Derleme

```bash
make
```

Alternatif olarak:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic bezier_yuzeyi.cpp -lglut -lGL -lGLU -o bezier_surface
```

## Çalıştırma

```bash
./bezier_surface
```

## Kullanım

- Sol fare tuşu ile bir kontrol noktasını tutup sürükleyin.
- Sağ fare tuşu menüsünden tel-kafes / dolu görünüm arasında geçin.
- Yön tuşları ile yüzeyi döndürün.

## Not

Bu repo içinde ekran görüntüsü veya otomatik GUI testi yok. Bu yüzden çalışma doğrulaması derleme üzerinden yapıldı; uygulamayı grafik oturumunda `./bezier_surface` ile açabilirsiniz.
