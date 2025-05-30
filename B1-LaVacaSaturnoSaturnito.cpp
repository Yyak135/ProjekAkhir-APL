#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <algorithm>
#include <limits> 

using namespace std;

#define MAX_PRODUK 100
#define MAX_USER 100
#define MAX_PESANAN 100
#define MAX_LAPORAN 100

struct DetailProduk {
    string warna;
    int stok;
};

struct Produk {
    int id;
    string nama;
    int harga;
    DetailProduk detail;
};

struct Pesanan {
    int idPesanan;
    string username;
    int idProduk;
    int jumlah;
    string status; 
};

struct User {
    string username;
    string password;
    int saldo;
};

struct Laporan {
    int idPesanan;
    string username;
    string namaProduk;
    int jumlah;
    int totalHarga;
};

Produk daftarProduk[MAX_PRODUK];
int jumlahProduk = 5;

User  pelanggan[MAX_USER];
int jumlahPelanggan = 0;

Pesanan daftarPesanan[MAX_PESANAN];
int jumlahPesanan = 0;
int idPesananCounter = 1;

string normalizeString(const string& str) {
    string normalized;
    for (char c : str) {
        if (!isspace(c)) {
            normalized += tolower(c);
        }
    }
    return normalized;
}

bool isDigitsOnly(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

bool isAlphaOnly(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), [](char c) {
        return isalpha(c) || isspace(c);
    });
}

void clearConsole() {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void inisialisasiProduk() {
    daftarProduk[0] = {1, "backpack rajut", 300000, {"pink", 20}};
    daftarProduk[1] = {2, "tote bag rajut", 125000, {"baby blue", 45}};
    daftarProduk[2] = {3, "sling bag rajut", 95000, {"hitam", 30}};
    daftarProduk[3] = {4, "hand bag rajut", 80000, {"cream", 15}};
    daftarProduk[4] = {5, "pouch rajut", 65000, {"maroon", 50}};
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

int cariUser (string username) {
    for (int i = 0; i < jumlahPelanggan; i++) {
        if (pelanggan[i].username == username) return i;
    }
    return -1;
}

bool login(User &user) {
    string username, password;
    cout << "Username: ";
    getline(cin, username);
    username = trim(username);
    
    if (username.empty()) {
        cout << "Username tidak boleh kosong!\n";
        return false;
    }

    cout << "Password: ";
    getline(cin, password);
    password = trim(password);

    if (username == "admin" && password == "admin123") {
        user.username = "admin";
        return true;
    }

    int idx = cariUser (username);
    if (idx != -1 && pelanggan[idx].password == password) {
        user = pelanggan[idx];
        return true;
    }
    
    cout << "Username atau password salah!\n";
    return false;
}

void simpanPelanggan(const User& user) {
    int idx = cariUser (user.username);
    if (idx != -1) pelanggan[idx] = user;
}

void registrasiPelanggan() {
    User baru;
    
    while (true) {
        cout << "Buat username: ";
        getline(cin, baru.username);
        baru.username = trim(baru.username);
        
        if (baru.username.empty()) {
            cout << "Username tidak boleh kosong!\n";
            continue;
        }
        
        if (baru.username == "admin") {
            cout << "Username 'admin' tidak diperbolehkan!\n";
            continue;
        }
        
        if (cariUser (baru.username) != -1) {
            cout << "Username sudah digunakan!\n";
            continue;
        }
        
        break;
    }

    while (true) {
        cout << "Buat password: ";
        getline(cin, baru.password);
        baru.password = trim(baru.password);
        
        if (baru.password.empty()) {
            cout << "Password tidak boleh kosong!\n";
            continue;
        }
        break;
    }

    baru.saldo = 0;
    pelanggan[jumlahPelanggan++] = baru;
    cout << "Registrasi berhasil!\n";
}

void tampilkanProduk() {
    cout << "\n+----+--------------------+------------+------+------------+\n";
    cout << "| ID | Nama Produk        | Harga      | Stok | Warna      |\n";
    cout << "+----+--------------------+------------+------+------------+\n";
    for (int i = 0; i < jumlahProduk; i++) {
        Produk &p = daftarProduk[i];
        cout << "| " << setw(2) << p.id << " | " << left << setw(18) << p.nama
             << " | " << right << setw(10) << p.harga
             << " | " << setw(4) << p.detail.stok
             << " | " << setw(10) << left << p.detail.warna << " |\n";
    }
    cout << "+----+--------------------+------------+------+------------+\n";
}

string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

string removeSpaces(string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

void cariProduk() {
    string keyword;
    cout << "Masukkan kata kunci nama produk (0 untuk kembali): ";
    getline(cin, keyword);
    keyword = trim(keyword);

    if (keyword == "0") {
        cout << "Pencarian dibatalkan.\n";
        return;
    }

    if (keyword.empty()) {
        cout << "\nError: Kata kunci tidak boleh kosong!\n";
        return;
    }

    if (any_of(keyword.begin(), keyword.end(), [](char c) { 
        return isdigit(c) || ispunct(c); 
    })) {
        cout << "\nError: Kata kunci hanya boleh mengandung huruf!\n";
        return;
    }

    string keywordClean = toLower(removeSpaces(keyword));
    bool ditemukan = false;

    cout << "\nHasil pencarian untuk \"" << keyword << "\":\n";

    for (int i = 0; i < jumlahProduk; i++) {
        string namaProdukClean = toLower(removeSpaces(daftarProduk[i].nama));

        if (namaProdukClean.find(keywordClean) != string::npos) {
            if (!ditemukan) {
                cout << "+----+--------------------+------------+------+------------+\n";
                cout << "| ID | Nama Produk        | Harga      | Stok | Warna      |\n";
                cout << "+----+--------------------+------------+------+------------+\n";
                ditemukan = true;
            }
            Produk &p = daftarProduk[i];
            cout << "| " << setw(2) << p.id << " | " << left << setw(18) << p.nama
                 << " | " << right << setw(10) << p.harga
                 << " | " << setw(4) << p.detail.stok
                 << " | " << setw(10) << left << p.detail.warna << " |\n";
        }
    }

    if (ditemukan) {
        cout << "+----+--------------------+------------+------+------------+\n";
    } else {
        cout << "Tidak ditemukan produk dengan kata kunci '" << keyword << "'\n";
    }
}

void konfirmasiPesanan() {
    if (jumlahPesanan == 0) {
        cout << "Belum ada pesanan.\n";
        return;
    }

    bool adaMenunggu = false;
    do {
        adaMenunggu = false;
        cout << "\nDaftar Pesanan:\n";
        cout << "+----+----------+--------------------+--------+-------------------------+\n";
        cout << "| ID | Username | Produk             | Jumlah | Status                  |\n";
        cout << "+----+----------+--------------------+--------+-------------------------+\n";
        
        int countMenunggu = 0;
        for (int i = 0; i < jumlahPesanan; i++) {
            string namaProduk = "";
            for (int j = 0; j < jumlahProduk; j++) {
                if (daftarProduk[j].id == daftarPesanan[i].idProduk) {
                    namaProduk = daftarProduk[j].nama;
                    break;
                }
            }
            cout << "| " << setw(2) << daftarPesanan[i].idPesanan
                 << " | " << setw(8) << daftarPesanan[i].username
                 << " | " << left << setw(18) << namaProduk
                 << " | " << right << setw(6) << daftarPesanan[i].jumlah
                 << " | " << setw(23) << daftarPesanan[i].status << " |\n";
                 
            if (daftarPesanan[i].status == "Menunggu konfirmasi") {
                adaMenunggu = true;
                countMenunggu++;
            }
        }
        cout << "+----+----------+--------------------+--------+-------------------------+\n";

        if (!adaMenunggu) {
            cout << "Tidak ada pesanan yang menunggu konfirmasi.\n";
            return;
        }

        string opsiInput;
        bool inputValid = false;
        while (!inputValid) {
            cout << "Apakah Anda ingin mengonfirmasi semua " << countMenunggu << " pesanan yang menunggu konfirmasi? (y/n/0 untuk batal): ";
            getline(cin, opsiInput);
            opsiInput = trim(opsiInput);
            
            if (opsiInput.empty()) {
                cout << "Input tidak boleh kosong!\n";
                continue;
            }
            
            if (opsiInput.length() > 1) {
                cout << "Hanya masukkan satu karakter (y/n/0)!\n";
                continue;
            }
            
            char opsi = tolower(opsiInput[0]);
            if (opsi == '0') {
                cout << "Konfirmasi dibatalkan.\n";
                return;
            }
            
            if (opsi != 'y' && opsi != 'n') {
                cout << "Input hanya boleh y, n, atau 0!\n";
                continue;
            }
            
            inputValid = true;

            if (opsi == 'y') {
                string konfirmasiUlang;
                cout << "Anda yakin ingin mengkonfirmasi semua " << countMenunggu << " pesanan? (y/0 untuk batal): ";
                getline(cin, konfirmasiUlang);
                konfirmasiUlang = trim(konfirmasiUlang);
                
                if (konfirmasiUlang == "0") {
                    cout << "Konfirmasi dibatalkan.\n";
                    continue;
                }
                
                if (tolower(konfirmasiUlang[0]) == 'y') {
                    for (int i = 0; i < jumlahPesanan; i++) {
                        if (daftarPesanan[i].status == "Menunggu konfirmasi") {
                            daftarPesanan[i].status = "Sudah dikonfirmasi";
                        }
                    }
                    cout << "Semua pesanan (" << countMenunggu << ") telah dikonfirmasi.\n";
                    adaMenunggu = false; 
                } else {
                    cout << "Konfirmasi dibatalkan.\n";
                    continue;
                }
            } else if (opsi == 'n') {
                string idInput;
                bool idValid = false;
                while (!idValid) {
                    cout << "Masukkan ID pesanan yang ingin dikonfirmasi (0 untuk batal): ";
                    getline(cin, idInput);
                    idInput = trim(idInput);
                    
                    if (idInput.empty()) {
                        cout << "ID tidak boleh kosong!\n";
                        continue;
                    }
                    
                    if (!all_of(idInput.begin(), idInput.end(), ::isdigit)) {
                        cout << "ID harus berupa angka!\n";
                        continue;
                    }
                    
                    int id = stoi(idInput);
                    if (id == 0) {
                        cout << "Konfirmasi dibatalkan.\n";
                        idValid = true;
                        continue;
                    }
                    
                    if (id <= 0) {
                        cout << "ID harus lebih besar dari 0!\n";
                        continue;
                    }
                    
                    bool ditemukan = false;
                    for (int i = 0; i < jumlahPesanan; i++) {
                        if (daftarPesanan[i].idPesanan == id && daftarPesanan[i].status == "Menunggu konfirmasi") {
                            daftarPesanan[i].status = "Sudah dikonfirmasi";
                            cout << "Pesanan berhasil dikonfirmasi.\n";
                            ditemukan = true;
                            countMenunggu--;
                            break;
                        }
                    }
                    
                    if (!ditemukan) {
                        cout << "ID pesanan tidak ditemukan atau sudah dikonfirmasi.\n";
                    } else {
                        idValid = true;
                    }
                }
            }
        }

        adaMenunggu = false;
        for (int i = 0; i < jumlahPesanan; i++) {
            if (daftarPesanan[i].status == "Menunggu konfirmasi") {
                adaMenunggu = true;
                break;
            }
        }

        if (!adaMenunggu) {
            cout << "Semua pesanan telah dikonfirmasi.\n";
            break;
        }

        string lanjutInput;
        bool lanjutValid = false;
        while (!lanjutValid) {
            cout << "Apakah Anda ingin melanjutkan konfirmasi pesanan? (y/n): ";
            getline(cin, lanjutInput);
            lanjutInput = trim(lanjutInput);
            
            if (lanjutInput.empty()) {
                cout << "Input tidak boleh kosong!\n";
                continue;
            }
            
            if (lanjutInput.length() > 1) {
                cout << "Hanya masukkan satu karakter (y/n)!\n";
                continue;
            }
            
            char lanjut = tolower(lanjutInput[0]);
            if (lanjut != 'y' && lanjut != 'n') {
                cout << "Input hanya boleh y atau n!\n";
                continue;
            }
            
            if (lanjut == 'n') {
                cout << "Kembali ke menu Admin.\n";
                return;
            }
        }

    } while (true);
}

void beliProduk(User &user) {
    string lanjutInput;
    do {
        tampilkanProduk();
        string idInput, jumlahInput;
        int id, jumlah;

        cout << "Masukkan ID produk yang ingin dibeli (0 untuk batal): ";
        getline(cin, idInput);
        idInput = trim(idInput);
        
        if (idInput.empty()) {
            cout << "ID produk tidak boleh kosong!\n";
            continue;
        }
        
        if (!all_of(idInput.begin(), idInput.end(), ::isdigit)) {
            cout << "ID produk harus berupa angka!\n";
            continue;
        }
        
        id = stoi(idInput);
        if (id < 0) {
            cout << "ID produk tidak boleh negatif!\n";
            continue;
        }

        if (id == 0) {
            cout << "Pembelian dibatalkan.\n";
            return;
        }

        cout << "Jumlah: ";
        getline(cin, jumlahInput);
        jumlahInput = trim(jumlahInput);
        
        if (jumlahInput.empty()) {
            cout << "Jumlah tidak boleh kosong!\n";
            continue;
        }
        
        if (!all_of(jumlahInput.begin(), jumlahInput.end(), ::isdigit)) {
            cout << "Jumlah harus berupa angka!\n";
            continue;
        }
        
        jumlah = stoi(jumlahInput);
        if (jumlah <= 0) {
            cout << "Jumlah harus lebih besar dari 0!\n";
            continue;
        }

        bool produkDitemukan = false;
        bool sudahAda = false;
        for (int i = 0; i < jumlahProduk; i++) {
            if (daftarProduk[i].id == id) {
                produkDitemukan = true;
                if (daftarProduk[i].detail.stok < jumlah) {
                    cout << "Stok tidak cukup.\n";
                    break;
                }
                for (int j = 0; j < jumlahPesanan; j++) {
                    if (daftarPesanan[j].username == user.username &&
                        daftarPesanan[j].idProduk == id &&
                        daftarPesanan[j].status == "Menunggu konfirmasi") {
                        daftarPesanan[j].jumlah += jumlah;
                        cout << "Pesanan untuk produk ini sudah ada. Jumlah pesanan ditambahkan.\n";
                        sudahAda = true;
                        break;
                    }
                }
                if (!sudahAda) {
                    daftarPesanan[jumlahPesanan++] = {idPesananCounter++, user.username, id, jumlah, "Menunggu konfirmasi"};
                    cout << "Pesanan dibuat, menunggu konfirmasi admin.\n";
                }
                break;
            }
        }
        if (!produkDitemukan) {
            cout << "Produk tidak ditemukan.\n";
        }

        cout << "Apakah Anda ingin membeli produk lain? (y/n): ";
        getline(cin, lanjutInput);
        lanjutInput = trim(lanjutInput);
        
        if (lanjutInput.empty()) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }
        
        if (lanjutInput.length() > 1) {
            cout << "Hanya masukkan satu karakter (y/n)!\n";
            continue;
        }
        
        char lanjut = tolower(lanjutInput[0]);
        if (lanjut != 'y' && lanjut != 'n') {
            cout << "Input hanya boleh y atau n!\n";
            continue;
        }
        
        if (lanjut != 'y') break;

    } while (true);

    cout << "Kembali ke menu pelanggan.\n";
}

void bayarPesanan(User &user) {
    if (jumlahPesanan == 0) {
        cout << "Belum ada pesanan.\n";
        return;
    }

    bool adaBelumBayar = false;
    do {
        adaBelumBayar = false;
        int countBelumBayar = 0;
        cout << "\nDaftar Pesanan Anda:\n";
        cout << "+----+--------------------+--------+------------+-------------------------+\n";
        cout << "| ID | Produk             | Jumlah | Total (Rp) | Status                  |\n";
        cout << "+----+--------------------+--------+------------+-------------------------+\n";

        int totalSemua = 0;
        for (int i = 0; i < jumlahPesanan; i++) {
            if (daftarPesanan[i].username == user.username && daftarPesanan[i].status == "Sudah dikonfirmasi") {
                string namaProduk = "";
                int hargaProduk = 0;
                for (int j = 0; j < jumlahProduk; j++) {
                    if (daftarProduk[j].id == daftarPesanan[i].idProduk) {
                        namaProduk = daftarProduk[j].nama;
                        hargaProduk = daftarProduk[j].harga;
                        totalSemua += daftarPesanan[i].jumlah * hargaProduk;
                        break;
                    }
                }
                countBelumBayar++;
                adaBelumBayar = true;
            }
        }

        for (int i = 0; i < jumlahPesanan; i++) {
            if (daftarPesanan[i].username == user.username) {
                string namaProduk = "";
                int hargaProduk = 0;
                for (int j = 0; j < jumlahProduk; j++) {
                    if (daftarProduk[j].id == daftarPesanan[i].idProduk) {
                        namaProduk = daftarProduk[j].nama;
                        hargaProduk = daftarProduk[j].harga;
                        break;
                    }
                }
                int total = daftarPesanan[i].jumlah * hargaProduk;
                cout << "| " << setw(2) << daftarPesanan[i].idPesanan
                     << " | " << left << setw(18) << namaProduk
                     << " | " << right << setw(6) << daftarPesanan[i].jumlah
                     << " | " << setw(10) << total
                     << " | " << setw(23) << daftarPesanan[i].status << " |\n";
            }
        }
        cout << "+----+--------------------+--------+------------+-------------------------+\n";

        if (!adaBelumBayar) {
            cout << "Tidak ada pesanan yang perlu dibayar.\n";
            return;
        }

        cout << "Total yang perlu dibayar: Rp" << totalSemua << endl;
        cout << "Saldo Anda: Rp" << user.saldo << endl;

        string opsiInput;
        bool inputValid = false;
        while (!inputValid) {
            cout << "Apakah Anda ingin membayar semua " << countBelumBayar << " pesanan yang sudah dikonfirmasi? (y/n/0 untuk batal): ";
            getline(cin, opsiInput);
            opsiInput = trim(opsiInput);
            
            if (opsiInput.empty()) {
                cout << "Input tidak boleh kosong!\n";
                continue;
            }
            
            if (opsiInput.length() > 1) {
                cout << "Hanya masukkan satu karakter (y/n/0)!\n";
                continue;
            }
            
            char opsi = tolower(opsiInput[0]);
            if (opsi == '0') {
                cout << "Pembayaran dibatalkan.\n";
                return;
            }
            
            if (opsi != 'y' && opsi != 'n') {
                cout << "Input hanya boleh y, n, atau 0!\n";
                continue;
            }
            
            inputValid = true;

            if (opsi == 'y') {
                string konfirmasiUlang;
                cout << "Anda yakin ingin membayar semua " << countBelumBayar << " pesanan sebesar Rp" << totalSemua << "? (y/0 untuk batal): ";
                getline(cin, konfirmasiUlang);
                konfirmasiUlang = trim(konfirmasiUlang);
                
                if (konfirmasiUlang == "0") {
                    cout << "Pembayaran dibatalkan.\n";
                    continue;
                }
                
                if (tolower(konfirmasiUlang[0]) == 'y') {
                    if (user.saldo >= totalSemua) {
                        for (int i = 0; i < jumlahPesanan; i++) {
                            if (daftarPesanan[i].username == user.username && daftarPesanan[i].status == "Sudah dikonfirmasi") {
                                user.saldo -= daftarPesanan[i].jumlah * daftarProduk[daftarPesanan[i].idProduk - 1].harga;
                                daftarProduk[daftarPesanan[i].idProduk - 1].detail.stok -= daftarPesanan[i].jumlah;
                                daftarPesanan[i].status = "Sudah dibayar";
                            }
                        }
                        simpanPelanggan(user);
                        cout << "Semua pesanan (" << countBelumBayar << ") berhasil dibayar.\n";
                        adaBelumBayar = false;
                    } else {
                        cout << "Saldo tidak cukup untuk membayar semua pesanan.\n";
                    }
                } else {
                    cout << "Pembayaran dibatalkan.\n";
                    continue;
                }
            } else if (opsi == 'n') {
                string idInput;
                bool idValid = false;
                while (!idValid) {
                    cout << "Masukkan ID pesanan yang ingin dibayar (0 untuk batal): ";
                    getline(cin, idInput);
                    idInput = trim(idInput);
                    
                    if (idInput.empty()) {
                        cout << "ID tidak boleh kosong!\n";
                        continue;
                    }
                    
                    if (!all_of(idInput.begin(), idInput.end(), ::isdigit)) {
                        cout << "ID harus berupa angka!\n";
                        continue;
                    }
                    
                    int id = stoi(idInput);
                    if (id == 0) {
                        cout << "Pembayaran dibatalkan.\n";
                        idValid = true;
                        continue;
                    }
                    
                    if (id <= 0) {
                        cout << "ID harus lebih besar dari 0!\n";
                        continue;
                    }
                    
                    bool ditemukan = false;
                    for (int i = 0; i < jumlahPesanan; i++) {
                        if (daftarPesanan[i].idPesanan == id && 
                            daftarPesanan[i].username == user.username && 
                            daftarPesanan[i].status == "Sudah dikonfirmasi") {
        
                            int totalPesanan = 0;
                            for (int j = 0; j < jumlahProduk; j++) {
                                if (daftarProduk[j].id == daftarPesanan[i].idProduk) {
                                    totalPesanan = daftarPesanan[i].jumlah * daftarProduk[j].harga;
                                    break;
                                }
                            }
                            
                            cout << "Total pesanan ID " << id << ": Rp" << totalPesanan << endl;
                            cout << "Saldo Anda: Rp" << user.saldo << endl;
                            
                            if (user.saldo >= totalPesanan) {
                                string konfirmasiBayar;
                                cout << "Anda yakin ingin membayar pesanan ID " << id << " sebesar Rp" << totalPesanan << "? (y/0 untuk batal): ";
                                getline(cin, konfirmasiBayar);
                                konfirmasiBayar = trim(konfirmasiBayar);
                                
                                if (konfirmasiBayar == "0") {
                                    cout << "Pembayaran dibatalkan.\n";
                                    break;
                                }
                                
                                if (tolower(konfirmasiBayar[0]) == 'y') {
                                    user.saldo -= totalPesanan;
                                    daftarProduk[daftarPesanan[i].idProduk - 1].detail.stok -= daftarPesanan[i].jumlah;
                                    daftarPesanan[i].status = "Sudah dibayar";
                                    simpanPelanggan(user);
                                    cout << "Pesanan berhasil dibayar.\n";
                                    countBelumBayar--;
                                    ditemukan = true;
                                    break;
                                } else {
                                    cout << "Pembayaran dibatalkan.\n";
                                    break;
                                }
                            } else {
                                cout << "Saldo tidak cukup untuk membayar pesanan ini.\n";
                                ditemukan = true;
                                break;
                            }
                        }
                    }
                    
                    if (!ditemukan) {
                        cout << "ID pesanan tidak ditemukan, bukan milik Anda, atau sudah dibayar.\n";
                    } else {
                        idValid = true;
                    }
                }
            }
        }

        adaBelumBayar = false;
        for (int i = 0; i < jumlahPesanan; i++) {
            if (daftarPesanan[i].username == user.username && daftarPesanan[i].status == "Sudah dikonfirmasi") {
                adaBelumBayar = true;
                break;
            }
        }

        if (!adaBelumBayar) {
            cout << "Semua pesanan yang perlu dibayar telah diproses.\n";
            break;
        }

        string lanjutInput;
        bool lanjutValid = false;
        while (!lanjutValid) {
            cout << "Apakah Anda ingin melanjutkan pembayaran pesanan? (y/n): ";
            getline(cin, lanjutInput);
            lanjutInput = trim(lanjutInput);
            
            if (lanjutInput.empty()) {
                cout << "Input tidak boleh kosong!\n";
                continue;
            }
            
            if (lanjutInput.length() > 1) {
                cout << "Hanya masukkan satu karakter (y/n)!\n";
                continue;
            }
            
            char lanjut = tolower(lanjutInput[0]);
            if (lanjut != 'y' && lanjut != 'n') {
                cout << "Input hanya boleh y atau n!\n";
                continue;
            }
            
            if (lanjut == 'n') {
                cout << "Kembali ke menu User.\n";
                return;
            }
        }

    } while (adaBelumBayar);
}

void topUp(User &user) {
    string nominalInput;
    int nominal;
    while (true) {
        cout << "Masukkan jumlah top up (0 untuk kembali): ";
        getline(cin, nominalInput);
        nominalInput = trim(nominalInput);

        if (nominalInput.empty()) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }
        bool hasInvalidChar = false;
        bool hasDecimalPoint = false;
        for (char c : nominalInput) {
            if (!isdigit(c)) {
                if (c == '.' || c == ',') {
                    hasDecimalPoint = true;
                } else if (c != '-' || &c != &nominalInput[0]) { 
                    hasInvalidChar = true;
                }
            }
        }

        if (hasInvalidChar) {
            cout << "Input harus berupa angka tanpa karakter khusus!\n";
            continue;
        }

        if (hasDecimalPoint) {
            cout << "Input harus berupa angka bulat tanpa titik atau koma!\n";
            continue;
        }

        try {
            nominal = stoi(nominalInput);
        } catch (const invalid_argument&) {
            cout << "Input harus berupa angka valid!\n";
            continue;
        } catch (const out_of_range&) {
            cout << "Nominal terlalu besar! Maksimal: " << numeric_limits<int>::max() << "\n";
            continue;
        }

        if (nominal < 0) {
            cout << "Nominal tidak boleh negatif.\n";
            continue;
        }

        if (nominal == 0) {
            return;
        }

        break;
    }

    user.saldo += nominal;
    simpanPelanggan(user);
    cout << "Top up berhasil. Saldo sekarang: Rp" << user.saldo << endl;
}

void cekSaldo(User &user) {
    cout << "Saldo Anda: Rp" << user.saldo << endl;
}

void swapLaporan(Laporan &a, Laporan &b) {
    Laporan temp = a;
    a = b;
    b = temp;
}

void laporanPenjualan() {
    do {
        Laporan laporanList[MAX_LAPORAN];
        int laporanCount = 0;
        for (int i = 0; i < jumlahPesanan; i++) {
            if (daftarPesanan[i].status == "Sudah dibayar") {
                string namaProduk = "";
                int hargaProduk = 0;
                for (int j = 0; j < jumlahProduk; j++) {
                    if (daftarProduk[j].id == daftarPesanan[i].idProduk) {
                        namaProduk = daftarProduk[j].nama;
                        hargaProduk = daftarProduk[j].harga;
                        break;
                    }
                }
                int total = daftarPesanan[i].jumlah * hargaProduk;
                laporanList[laporanCount++] = {
                    daftarPesanan[i].idPesanan, 
                    daftarPesanan[i].username, 
                    namaProduk, 
                    daftarPesanan[i].jumlah, 
                    total
                };
            }
        }

        if (laporanCount == 0) {
            cout << "\nTidak ada data penjualan.\n";
            pause();
            return;
        }
        string pilihanInput;
        int pilihanSort;
        bool inputValid = false;
        
        while (!inputValid) {
            cout << "\nPilih Format Laporan Penjualan:\n";
            cout << "1. Berdasarkan Harga Termahal\n";
            cout << "2. Berdasarkan Stok Terjual Terbanyak\n";
            cout << "0. Kembali\n";
            cout << "Pilihan: ";
            getline(cin, pilihanInput);
            pilihanInput = trim(pilihanInput);
            
            if (pilihanInput.empty()) {
                cout << "Input tidak boleh kosong!\n";
                continue;
            }
            
            if (!all_of(pilihanInput.begin(), pilihanInput.end(), ::isdigit)) {
                cout << "Input harus berupa angka!\n";
                continue;
            }
            
            pilihanSort = stoi(pilihanInput);
            
            if (pilihanSort < 0 || pilihanSort > 2) {
                cout << "Pilihan harus antara 0-2!\n";
                continue;
            }
            
            inputValid = true;
        }

        if (pilihanSort == 0) return;
        for (int i = 0; i < laporanCount - 1; i++) {
            for (int j = 0; j < laporanCount - 1 - i; j++) {
                bool swapNeeded = false;
                if (pilihanSort == 1) {
                    swapNeeded = laporanList[j].totalHarga < laporanList[j + 1].totalHarga;
                } else if (pilihanSort == 2) {
                    swapNeeded = laporanList[j].jumlah < laporanList[j + 1].jumlah;
                }
                if (swapNeeded) {
                    swapLaporan(laporanList[j], laporanList[j + 1]);
                }
            }
        }

        cout << "\n+----+----------+--------------------+--------+------------+\n";
        cout << "| ID | Username | Produk             | Jumlah | Total (Rp) |\n";
        cout << "+----+----------+--------------------+--------+------------+\n";

        for (int i = 0; i < laporanCount; i++) {
            cout << "| " << setw(2) << laporanList[i].idPesanan 
                 << " | " << setw(8) << laporanList[i].username
                 << " | " << left << setw(18) << laporanList[i].namaProduk
                 << " | " << right << setw(6) << laporanList[i].jumlah
                 << " | " << setw(10) << laporanList[i].totalHarga << " |\n";
        }
        cout << "+----+----------+--------------------+--------+------------+\n";

        int totalPenjualan = 0;
        for (int i = 0; i < laporanCount; i++) {
            totalPenjualan += laporanList[i].totalHarga;
        }
        cout << "| " << setw(42) << "Total Penjualan" << " | " << setw(10) << totalPenjualan << " |\n";
        cout << "+--------------------------------------------+------------+\n";
        string lanjutInput;
        bool lanjutValid = false;
        
        while (!lanjutValid) {
            cout << "\nLihat laporan lagi? (y/n): ";
            getline(cin, lanjutInput);
            lanjutInput = trim(lanjutInput);
            
            if (lanjutInput.empty()) {
                cout << "Input tidak boleh kosong!\n";
                continue;
            }
            
            if (lanjutInput.length() > 1) {
                cout << "Hanya masukkan satu karakter (y/n)!\n";
                continue;
            }
            
            char lanjut = tolower(lanjutInput[0]);
            if (lanjut != 'y' && lanjut != 'n') {
                cout << "Input hanya boleh y atau n!\n";
                continue;
            }
            
            if (lanjut == 'n') {
                return;
            }
            
            lanjutValid = true;
        }
    } while (true);
}

void tambahProduk() {
    if (jumlahProduk >= MAX_PRODUK) {
        cout << "Kapasitas produk penuh, tidak dapat menambah produk baru.\n";
        return;
    }

    Produk p;
    p.id = (jumlahProduk > 0) ? daftarProduk[jumlahProduk - 1].id + 1 : 1;

    while (true) {
        cout << "Masukkan nama produk (0 untuk kembali): ";
        getline(cin, p.nama);
        p.nama = trim(p.nama);
        if (p.nama == "0") return;
        if (p.nama.empty()) { cout << "Nama tidak boleh kosong!\n"; continue; }
        if (!isAlphaOnly(p.nama)) { cout << "Nama hanya boleh huruf dan spasi!\n"; continue; }
        break;
    }

    string hargaInput;
    while (true) {
        cout << "Masukkan harga produk (0 untuk kembali): ";
        getline(cin, hargaInput);
        hargaInput = trim(hargaInput);
        if (hargaInput == "0") return;
        if (hargaInput.empty()) { cout << "Input tidak boleh kosong!\n"; continue; }
        if (!isDigitsOnly(hargaInput)) { cout << "Harga harus angka bulat positif!\n"; continue; }
        try { p.harga = stoi(hargaInput); } catch (...) { cout << "Input harga tidak valid!\n"; continue; }
        break;
    }

    string stokInput;
    while (true) {
        cout << "Masukkan stok produk (0 untuk kembali): ";
        getline(cin, stokInput);
        stokInput = trim(stokInput);
        if (stokInput == "0") return;
        if (stokInput.empty()) { cout << "Input tidak boleh kosong!\n"; continue; }
        if (!isDigitsOnly(stokInput)) { cout << "Stok harus angka bulat positif!\n"; continue; }
        try { p.detail.stok = stoi(stokInput); } catch (...) { cout << "Input stok tidak valid!\n"; continue; }
        break;
    }

    while (true) {
        cout << "Masukkan warna produk (0 untuk kembali): ";
        getline(cin, p.detail.warna);
        p.detail.warna = trim(p.detail.warna);
        if (p.detail.warna == "0") return;
        if (p.detail.warna.empty()) { cout << "Warna tidak boleh kosong!\n"; continue; }
        if (!isAlphaOnly(p.detail.warna)) { cout << "Warna hanya boleh huruf dan spasi!\n"; continue; }
        break;
    }

    string normalizedNama = normalizeString(p.nama);
    string normalizedWarna = normalizeString(p.detail.warna);
    for (int i = 0; i < jumlahProduk; i++) {
        if (normalizeString(daftarProduk[i].nama) == normalizedNama &&
            normalizeString(daftarProduk[i].detail.warna) == normalizedWarna) {
            cout << "Produk \"" << daftarProduk[i].nama << "\" warna \"" << daftarProduk[i].detail.warna 
                 << "\" sudah ada.\nPenambahan dibatalkan.\n";
            return;
        }
    }

    daftarProduk[jumlahProduk++] = p;
    cout << "Produk berhasil ditambahkan.\n";
}

void perbaruiProduk() {
    tampilkanProduk();
    if (jumlahProduk == 0) {
        return;
    }

    string idInput;
    int id;
    while (true) {
        cout << "Masukkan ID produk yang ingin diperbarui (0 untuk kembali): ";
        getline(cin, idInput);
        idInput = trim(idInput);

        if (idInput.empty()) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }

        if (idInput[0] == '-') {
            cout << "ID tidak boleh negatif!\n";
            continue;
        }

        if (!all_of(idInput.begin(), idInput.end(), ::isdigit)) {
            cout << "Input harus berupa angka!\n";
            continue;
        }

        id = stoi(idInput);
        break;
    }

    if (id == 0) return;

    int idx = -1;
    for (int i = 0; i < jumlahProduk; i++) {
        if (daftarProduk[i].id == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        cout << "Produk dengan ID tersebut tidak ditemukan.\n";
        return;
    }

    string stokInput;
    int stokBaru;
    while (true) {
        cout << "Masukkan stok baru (0 untuk kembali): ";
        getline(cin, stokInput);
        stokInput = trim(stokInput);

        if (stokInput.empty()) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }

        if (stokInput[0] == '-') {
            cout << "Stok tidak boleh negatif!\n";
            continue;
        }

        if (!all_of(stokInput.begin(), stokInput.end(), ::isdigit)) {
            cout << "Input harus berupa angka!\n";
            continue;
        }

        stokBaru = stoi(stokInput);
        if (stokBaru == 0) return;
        break;
    }
    daftarProduk[idx].detail.stok = stokBaru;

    string hargaInput;
    int hargaBaru;
    while (true) {
        cout << "Masukkan harga baru (0 untuk kembali): ";
        getline(cin, hargaInput);
        hargaInput = trim(hargaInput);

        if (hargaInput.empty()) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }

        if (hargaInput[0] == '-') {
            cout << "Harga tidak boleh negatif!\n";
            continue;
        }

        if (!all_of(hargaInput.begin(), hargaInput.end(), ::isdigit)) {
            cout << "Input harus berupa angka!\n";
            continue;
        }

        hargaBaru = stoi(hargaInput);
        if (hargaBaru == 0) return;
        break;
    }
    daftarProduk[idx].harga = hargaBaru;

    cout << "Produk berhasil diperbarui.\n";
}

void hapusProduk() {
    tampilkanProduk();
    if (jumlahProduk == 0) {
        return;
    }

    string idInput;
    int id;
    while (true) {
        cout << "Masukkan ID produk yang ingin dihapus (0 untuk kembali): ";
        getline(cin, idInput);
        idInput = trim(idInput);
        
        if (idInput.empty()) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }
        
        if (!all_of(idInput.begin(), idInput.end(), ::isdigit)) {
            cout << "Input harus berupa angka!\n";
            continue;
        }
        
        id = stoi(idInput);
        break;
    }

    if (id == 0) return;

    int idx = -1;
    for (int i = 0; i < jumlahProduk; i++) {
        if (daftarProduk[i].id == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        cout << "Produk dengan ID tersebut tidak ditemukan.\n";
        return;
    }

    for (int i = idx; i < jumlahProduk - 1; i++) {
        daftarProduk[i] = daftarProduk[i + 1];
    }
    jumlahProduk--;

    for (int i = 0; i < jumlahProduk; i++) {
        daftarProduk[i].id = i + 1;
    }

    cout << "Produk berhasil dihapus.\n";
}

int main() {
    inisialisasiProduk();
    string pilihInput;
    int pilih;
    do {
        clearConsole();
        cout << "\n+------------------------------+\n";
        cout << "|          Menu Utama          |\n";
        cout << "+------------------------------+\n";
        cout << "1. Login\n2. Registrasi\n3. Keluar\nPilihan: ";
        
        getline(cin, pilihInput);
        pilihInput = trim(pilihInput);
        
        if (pilihInput.empty()) {
            cout << "Input tidak boleh kosong!\n";
            pause();
            continue;
        }

        if (pilihInput[0] == '-') {
            cout << "Input tidak boleh negatif!\n";
            pause();
            continue;
        }

        if (!all_of(pilihInput.begin(), pilihInput.end(), ::isdigit)) {
            cout << "Input harus berupa angka!\n";
            pause();
            continue;
        }

        pilih = stoi(pilihInput);

        if (pilih == 1) {
            User user;
            int percobaan = 0;
            bool berhasil = false;
            while (percobaan < 3) {
                if (login(user)) {
                    berhasil = true;
                    break;
                }
                percobaan++;
                if (percobaan < 3) {
                    cout << "Login gagal! Percobaan ke-" << percobaan << "/3\n";
                }
            }
            if (!berhasil) {
                cout << "Terlalu banyak percobaan gagal. Program dihentikan.\n";
                return 0;
            }
            
            if (user.username == "admin") {
                string pilihInputAdmin; 
                int pil;
                do {
                    clearConsole();
                    cout << "\n=== Menu Admin ===\n";
                    cout << "1. Lihat Produk\n2. Tambah Produk\n3. Perbarui Produk\n4. Hapus Produk\n5. Konfirmasi Pesanan\n6. Laporan Penjualan\n7. Logout\nPilihan: ";
                    
                    getline(cin, pilihInputAdmin);
                    pilihInputAdmin = trim(pilihInputAdmin);

                    if (pilihInputAdmin.empty()) {
                        cout << "Input tidak boleh kosong.\n";
                        pause();
                        continue;
                    }

                    if (pilihInputAdmin[0] == '-') {
                        cout << "Input tidak boleh negatif.\n";
                        pause();
                        continue;
                    }

                    if (!all_of(pilihInputAdmin.begin(), pilihInputAdmin.end(), ::isdigit)) {
                        cout << "Input harus berupa angka.\n";
                        pause();
                        continue;
                    }

                    pil = stoi(pilihInputAdmin);

                    switch (pil) {
                        case 1:
                            tampilkanProduk();
                            pause();
                            break;
                        case 2:
                            tambahProduk();
                            pause();
                            break;
                        case 3:
                            perbaruiProduk();
                            pause();
                            break;
                        case 4:
                            hapusProduk();
                            pause();
                            break;
                        case 5:
                            konfirmasiPesanan();
                            pause();
                            break;
                        case 6:
                            laporanPenjualan();
                            pause();
                            break;
                        case 7:
                            cout << "Logout berhasil.\n";
                            break;
                        default:
                            cout << "Pilihan tidak valid.\n";
                            pause();
                    }
                } while (pil != 7);
            } else {
                int pil;
                do {
                    clearConsole();
                    cout << "\n=== Menu Pelanggan ===\n";
                    cout << "1. Lihat Produk\n2. Cari Produk\n3. Beli Produk\n4. Bayar Pesanan\n5. Top Up Saldo\n6. Cek Saldo\n7. Logout\nPilihan: ";
                    
                    if (!(cin >> pil)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Input tidak valid. Harap masukkan angka.\n";
                        pause();
                        continue;
                    }

                    if (pil < 0) {
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Input tidak boleh negatif.\n";
                        pause();
                        continue;
                    }

                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    switch (pil) {
                        case 1:
                            tampilkanProduk();
                            pause();
                            break;
                        case 2:
                            cariProduk();
                            pause();
                            break;
                        case 3:
                            beliProduk(user);
                            pause();
                            break;
                        case 4:
                            bayarPesanan(user);
                            pause();
                            break;
                        case 5:
                            topUp(user);
                            pause();
                            break;
                        case 6:
                            cekSaldo(user);
                            pause();
                            break;
                        case 7:
                            cout << "Logout berhasil.\n";
                            break;
                        default:
                            cout << "Pilihan tidak valid.\n";
                            pause();
                    }
                } while (pil != 7);
            }
        } else if (pilih == 2) {
            registrasiPelanggan();
            pause();
        } else if (pilih == 3) {
            cout << "Terima kasih.\n";
        } else {
            cout << "Pilihan tidak valid.\n";
            pause();
        }
    } while (pilih != 3);
    return 0;
}