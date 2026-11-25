<div align="right">
  <b>Türkçe</b> | <a href="README.en.md">English</a>
</div>

# CM (C-Mat / C-Mantık)

**CM**, matematiksel ifade rahatlığı ile yalın mantıksal akışı birleştiren, C tabanlı ve yüksek performanslı bir programlama dilidir. Standart C'nin tip bildirimleri, süslü parantez ve noktalı virgül kalabalığını ortadan kaldırır; kaynak kodu (`.cm`) doğrudan saf C üzerinden makine koduna derler.

---

## Kurulum ve Çalıştırma

### 1. Derleyici Motorunu Hazırlama
```bash
gcc cm_derle.c -o cm
```
*(İsteğe bağlı) Sistem genelinde her dizinden `cm dosya.cm` komutuyla çalıştırmak için:*
```bash
sudo cp cm /usr/local/bin/
```

### 2. `.cm` Dosyasını Çalıştırma
```bash
./cm x.cm
# Veya sistem yoluna eklendiyse:
cm x.cm
```

---

## CM Söz Dizimi ve Komut Rehberi

### 1. Kütüphane ve Değişken Tanımlama
* **`call <library>`**: Harici veya standart kütüphaneleri dahil eder.
* **Çoklu Atama (`x = 10, y = 20`)**: Tek satırda virgülle ayrılmış dinamik değişken tanımlaması yapar.
* **Otomatik Hedef Kurtarma (`target = id("")`)**: `target` boş/tanımsız bırakıldığında `save(target)` çalıştırılırsa sistem hata verip durmak yerine otomatik varsayılan bir hedef klasörü oluşturup veriyi oraya yönlendirir.

```text
call <math>
x = 10, y = 20, target = id("")
```

### 2. Koşul, Mantık Kapıları ve Blok Akışı
* **Ok Operatörü (`->`) ve Zincirleme Koşullar**: Koşul gövdelerini başlatır ve `-> and` ile tek satırda mantıksal zincirler kurar.
* **Matematiksel Aralık Eşitsizlikleri (`10 > x >= 5`)**: C'deki gibi parçalamaya gerek kalmadan doğal matematiksel aralık kıyaslaması yapar.
* **Blok Yönlendirme (`return block("main")`)**: Koşul sağlandığında akışı doğrudan tanımlı etikete/bloğa aktarır.
* **Blok ve Program Sonlandırma (`end` vs `end.`)**:
  * **`end`**: Sadece içinde bulunulan ifadeyi/bloğu sonlandırır.
  * **`end.`**: Tüm programın akışını tamamen bitirir.

```text
if x == 1 ->
    save(target)
end id("main")

if x == 4 -> and y > 50 ->
    return block("main")
    print (y * id(""))
    save(target) id("k")
end id(random)
```

### 3. Döngüler ve Aralık İterasyonu (`loop` & `frange`)
* **`loop(id("m"), id("a"))`**: Belirtilen iki kimlik bloğu arasında döngü kurar.
* **`loop(frange.(id("m")-id("a")))`**: Başlangıç ve bitiş kimlikleri/değişkenleri arasındaki tüm aralık boyunca (artan veya azalan yönde) otomatik iterasyon yapar.

```text
if 10 > x >= 5 ->
    loop(frange.(id("m")-id("a")))

if id("m") or id("a") == 0 ->
    end id("z")
```

### 4. Zorlama Eşitleme ve Matematiksel Biçimlendirme (`make.`)
* **`make.(x == y)`**: Koşul sağlandığında `x` değerini `y` değerine eşit olmaya zorlar.
* **Bileşik Eylem ve Üslü Çıktı (`print(f^(x * y))`)**: Tek satırda `and` ile birden fazla eylemi birleştirir ve matematiksel üs notasyonunu doğrudan hesaplar.

```text
if x >= 10 ->
    make.(x == y) and print(f^(x * y))
end. id(random)
```

### 5. Kimlik Sistemi (`id(...)`) Kuralları
CM dilinde sabit isimlerle sistem komutlarının çakışmaması için tırnak standardı uygulanır:

| Yazım | Tür | Davranış |
| :--- | :--- | :--- |
| **`id("main")`** | Sabit İsim | Tırnak içindeki ifadeyi birebir blok/etiket adı olarak atar. |
| **`id(number)`** | Sıralı Sayı | `1, 2, 3...` şeklinde otomatik artan sayısal kimlik üretir. |
| **`id(words)`** | Sıralı Harf | Alfabetik sırayla kimlik verir; tek harf bittiğinde çift harfe (`AA`, `AB`, `SE`...) geçer. |
| **`id(random)`** | Rastgele | Doğrudan rastgele harf veya sayı tabanlı benzersiz kimlik atar. |
| **`id(random_number)`** | Rastgele Sayı | `0` ile `1000` arasında rastgele sayı üretir; aralık dolduğunda basamak kapasitesini otomatik artırır. |
| **`id(random_words)`** | Rastgele Harf | Rastgele harf dizisi üretir; kombinasyonlar tükendiğinde karakter uzunluğunu artırarak devam eder. |

### 6. Bağlantı ve Kaynak Tanımlama (`href`)
Virgülsüz nitelik standartına uygun olarak dış bağlantıları veya dosya yollarını görünen isim ve kimlik etiketiyle birlikte bağlar:

```text
href "[https://ornek.com/veri](https://ornek.com/veri)" show("Veri Kaynağı") id("etiket") /href
```
````[cite: 10]

Bu `README.md` dosyasını repoya eklediğinde projenin hem mimarisi hem de her bir komutun kullanım mantığı dışarıdan bakan biri (veya ileride senin kendi referansın) için net bir başvuru kaynağına dönüşmüş olur.
