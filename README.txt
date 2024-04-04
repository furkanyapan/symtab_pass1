Not: Kodun içeriğinde de yorum satırları ile açıklamalar yer almaktadır.
1- struct SYMTAB, sembol tablosunda (ST array'inde) her bir sembol için bir kayıt oluşturmak için kullanılır. SYMTAB yapısı label, addr olmak üzere iki elemandan oluşuyor.

2- struct OPTAB, opcode tablosunda (OT array'inde) her bir opcode için bir kayıt oluşturmak için kullanılır. OPTAB yapısı opcode, hexcode olmak üzere iki elemendan oluşuyor.

3- read_line() fonksiyonu, input dosyasından bir satır okur ve bu satırı label, opcode ve operand olmak üzere parçalara ayırır.
	a. strcpy(t1, "");, strcpy(t2, ""); ve strcpy(t3, ""); satırları, t1, t2 ve t3 dizilerini boşaltır. Bu diziler, geçici olarak okunan satırın parçalarını saklamak için kullanılır.
	b. fscanf(inputFile, "%s", t1); satırı, girdi dosyasından bir satır okur ve t1 dizisine ilk kelimeyi kopyalar.
	c. if (fgetc(inputFile) != '\n') satırı, okunan satırın sonuna gelinip gelinmediğini kontrol eder. 
	d. İkinci ve üçüncü kelimeleri (t2 ve t3) okumak için benzer adımlar izlenir.
	e. Okunan kelimeler, boş olup olmadıkları kontrol edilerek uygun değişkenlere kopyalanır. Eğer t1, t2 ve t3 dolu ise, bu üç kelime sırasıyla label, opcode ve operand olarak atanır. Eğer sadece t1 ve t2 dolu ise, label boş olarak kabul edilir. Eğer sadece t1 dolu ise, etiket ve operand boş olarak kabul edilir.

4- int main() fonksiyonu
Giriş dosyalarını aç: input.txt, optab.txt.
Çıkış dosyası oluştur/üzerine yaz: symtab.txt.

Opcode tablosunu okuyarak 'OT' dizisine yükle.
	a. while(1) dögüsü ile sonsuz bir dögü başlatılır.
	b. Her döngü adımında 'o' değişkeni bir arttırılarak 'OT' dizisindeki bir sonraki boş hücreye geçiş için kullanılır.
	c. fscanf(optableFile, "%s%s", OT[o].opcode, OT[o].hexcode); satırı, optableFile dosyasından bir satır okur. Bu satırda iki kelime bulunmalıdır: birincisi opcode, ikincisi ise opcode'un karşılık geldiği onaltılık (hexadecimal) kod. Bu değerler OT[o].opcode ve OT[o].hexcode dizilerine sırasıyla atanır.
	d. if (feof(optableFile)) break; satırı, dosyanın sonuna gelinip gelinmediğini kontrol eder. Dosyanın sonuna gelindiğinde yani feof fonksiyonu true döndüğünde break ifadesiyle döngüden çıkılır.

Giriş dosyasından ilk satırı oku.
Eğer ilk opcode "START" ise:
	Belirtilen başlangıç adresine locctr'yi ayarla.
	Giriş dosyasından bir sonraki satırı oku.
Değilse:
	locctr'yi 0 olarak ayarla.

Opcode "END" olduğu sürece döngüyü tekrarla:
	Mevcut satırda bir label varsa:
		Label'ın sembol tablosunda (ST) olup olmadığını kontrol et:
			Evet ise, bir hata mesajı yazdır ve çık.
			Hayır ise, etiketi sembol tablosuna, ilgili adresiyle birlikte ekle.
	Opcode'u opcode tablosunda (OPTAB) ara.
	Opcode bulunursa:
		locctr'yi 3 artır.
	Opcode bulunmazsa:
		Opcode'u kontrol et ve buna göre işle:
			Opcode "WORD" ise, locctr'yi 3 artır.
			Opcode "RESW" ise, locctr'yi operand değerinin 3 katı kadar artır.
			Opcode "RESB" ise, locctr'yi operand değeri kadar artır.
			Opcode "BYTE" ise:
				Operandın uzunluğunu belirle:
					Operand 'C' ile başlıyorsa, uzunluktan 3 çıkar.
					Operand 'X' ile başlıyorsa, kalan uzunluğu 3'ten çıkar ve 2'ye böl.
				locctr'yi operand uzunluğu kadar artır.
	Giriş dosyasından bir sonraki satırı oku.

Sembol tablosunun (ST) içeriğini symtab.txt'ye yaz.

Tüm dosyaları kapat.

Input: Örnek Program
COPY   START  1000
FIRST  STL    RETADR
CLOOP  JSUB   RDREC
       LDA    LENGTH
       COMP   ZERO
       JEQ    ENDFIL
       JSUB   WRREC
       J      CLOOP
ENDFIL LDA    EOF
       STA    BUFFER
       LDA    THREE
       STA    LENGTH
       JSUB   WRREC
       LDL    RETADR
       RSUB
EOF    BYTE   C'EOF'
THREE  WORD   3
ZERO   WORD   0
RETADR RESW   1
LENGTH RESW   1
BUFFER RESB   4096
RDREC  LDX    ZERO
       LDA    ZERO
RLOOP  TD     INPUT
       JEQ    RLOOP
       RD     INPUT
       COMP   ZERO
       JEQ    EXIT
       STCH   BUFFER,X
       TIX    MAXLEN
       JLT    RLOOP
EXIT   STX    LENGTH
       RSUB
INPUT  BYTE   X'F1'
MAXLEN WORD   4096
WRREC  LDX    ZERO
WLOOP  TD     OUTPUT
       JEQ    WLOOP
       LDCH   BUFFER,X
       WD     OUTPUT
       TIX    LENGTH
       JLT    WLOOP
       RSUB
OUTPUT BYTE   X'05'
       END    FIRST

OUTPUT: SEMBOL TABLOSU
1000	FIRST
1003	CLOOP
1015	ENDFIL
102a	EOF
102d	THREE
1030	ZERO
1033	RETADR
1036	LENGTH
1039	BUFFER
2039	RDREC
203f	RLOOP
2057	EXIT
205d	INPUT
205e	MAXLEN
2061	WRREC
2064	WLOOP
2079	OUTPUT
