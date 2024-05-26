#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

//! Deklarasi batas maksimal
const int MAX_PASIEN = 100;
const int MAX_DOKTER = 50;

//! Deklarasi harga kontrol & obat
const int HARGA_KONTROL = 50000;
const int HARGA_OBAT = 20000;

void loadingAnimasi()
{
    cout.flush();
    const char animationChars[] = {'|', '/', '-', '\\'};
    const int numChars = sizeof(animationChars) / sizeof(animationChars[0]);

    for (int i = 0; i < 10; ++i)
    {
        cout << animationChars[i % numChars] << "\b";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(30));
    }
    cout << "" << endl;
}

void clearTerminal()
{
    system("cls");
}

struct Pasien
{
    string nama;
    string tanggalLahir;
    bool asuransiBPJS;
    string jenisGejala;
    string lokasiGejala;
    string durasiGejala;
    string intensitasGejala;
    string jenisPenyakit;
    int obat;
    int jumlahKontrol;
};

string spesialisDokter[MAX_DOKTER] = {"jantung", "paru-paru", "kulit", "mata", "gigi"};
int jumlahSpesialis = 5;

Pasien databasePasien[MAX_PASIEN];
int jumlahPasien = 0;

bool isValidInput(int input)
{
    return cin.good() && input >= 1 && input <= 3;
}

bool isValidString(const string &str)
{
    bool onlySpaces = true;
    for (char c : str)
    {
        if (!isalpha(c) && c != ' ')
        {
            return false;
        }
        if (isalpha(c))
        {
            onlySpaces = false;
        }
    }
    return !onlySpaces;
}

bool isValidNumber(const string &str)
{
    for (char c : str)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

bool isValidDate(const string &tanggal)
{
    if (tanggal.length() != 10 || tanggal[2] != '/' || tanggal[5] != '/')
    {
        return false;
    }

    int hari = stoi(tanggal.substr(0, 2));
    int bulan = stoi(tanggal.substr(3, 2));
    int tahun = stoi(tanggal.substr(6));

    if (tahun < 1900 || tahun > 2023)
    {
        return false;
    }

    if (bulan < 1 || bulan > 12)
    {
        return false;
    }

    switch (bulan)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (hari < 1 || hari > 31)
        {
            return false;
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (hari < 1 || hari > 30)
        {
            return false;
        }
        break;
    case 2:
        bool kabisat = (tahun % 4 == 0 && tahun % 100 != 0) || (tahun % 400 == 0);
        if (kabisat && (hari < 1 || hari > 29))
        {
            return false;
        }
        else if (!kabisat && (hari < 1 || hari > 28))
        {
            return false;
        }
        break;
    }

    return true;
}

// int PEMBATASPASIEN = 0;

void registrasi()
{
    cout << "==============================" << endl;
    cout << "|         Registrasi         |" << endl;
    cout << "==============================" << endl;

    string nama;
    while (true)
    {
        cout << "Nama lengkap: ";
        cin.ignore();
        getline(cin, nama);
        if (nama.empty())
        {
            cout << "Nama lengkap tidak boleh kosong." << endl;
        }
        else if (!isValidString(nama))
        {
            cout << "Nama lengkap tidak boleh mengandung angka atau simbol." << endl;
        }
        else
        {
            break;
        }
    }

    string tanggalLahir;
    while (true)
    {
        cout << "Tanggal lahir (dd/mm/yyyy): ";
        getline(cin, tanggalLahir);
        if (!isValidDate(tanggalLahir))
        {
            cout << "Tanggal lahir tidak valid. Format yang benar adalah dd/mm/yyyy." << endl;
        }
        else
        {
            break;
        }
    }

    char asuransiBPJS;
    bool memilikiBPJS;
    while (true)
    {
        cout << "Asuransi BPJS (y/n): ";
        cin >> asuransiBPJS;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (asuransiBPJS == 'y' || asuransiBPJS == 'Y')
        {
            memilikiBPJS = true;
            break;
        }
        else if (asuransiBPJS == 'n' || asuransiBPJS == 'N')
        {
            memilikiBPJS = false;
            break;
        }
        else
        {
            cout << "Input tidak valid untuk Asuransi BPJS. Harus 'y' atau 'n'." << endl;
        }
    }

    string jenisGejala;
    while (true)
    {
        cout << "Jenis gejala (sakit, nyeri, pusing, dll.): ";
        getline(cin, jenisGejala);
        if (jenisGejala.empty())
        {
            cout << "Jenis gejala tidak boleh kosong." << endl;
        }
        else if (!isValidString(jenisGejala))
        {
            cout << "Jenis gejala tidak boleh mengandung angka atau simbol." << endl;
        }
        else
        {
            break;
        }
    }

    string lokasiGejala;
    while (true)
    {
        cout << "Lokasi gejala (bagian tubuh yang terkena): ";
        getline(cin, lokasiGejala);
        if (lokasiGejala.empty())
        {
            cout << "Lokasi gejala tidak boleh kosong." << endl;
        }
        else if (!isValidString(lokasiGejala))
        {
            cout << "Lokasi gejala tidak boleh mengandung angka atau simbol." << endl;
        }
        else
        {
            break;
        }
    }

    string durasiGejala;
    while (true)
    {
        cout << "Durasi gejala sudah berlangsung berapa lama (hari): ";
        getline(cin, durasiGejala);
        if (durasiGejala.empty())
        {
            cout << "Durasi gejala tidak boleh kosong." << endl;
        }
        else if (!isValidNumber(durasiGejala))
        {
            cout << "Durasi gejala tidak boleh mengandung string atau simbol." << endl;
        }
        else
        {

            int durasi = stoi(durasiGejala);
            if (durasi < 1)
            {
                cout << "Durasi gejala harus lebih dari 0 hari." << endl;
            }
            else
            {
                break;
            }
        }
    }

    string intensitasGejala;
    while (true)
    {
        cout << "Intensitas gejala (ringan, sedang, berat): ";
        getline(cin, intensitasGejala);
        if (intensitasGejala.empty())
        {
            cout << "Intensitas gejala tidak boleh kosong." << endl;
        }
        else if (!isValidString(intensitasGejala))
        {
            cout << "Intensitas gejala tidak boleh mengandung angka atau simbol." << endl;
        }
        else if (intensitasGejala != "ringan" && intensitasGejala != "sedang" && intensitasGejala != "berat")
        {
            cout << "Intensitas gejala harus ringan, sedang, atau berat." << endl;
        }
        else
        {
            break;
        }
    }

    clearTerminal();

    if (jumlahPasien < MAX_PASIEN)
    {
        databasePasien[jumlahPasien] = {nama, tanggalLahir, memilikiBPJS, jenisGejala, lokasiGejala, durasiGejala, intensitasGejala};
        ofstream file("a-belumbayar.csv", ios::app);
        if (file.is_open())
        {
            file << nama << ","
                 << tanggalLahir << ","
                 << (memilikiBPJS ? "true" : "false") << ","
                 << jenisGejala << ","
                 << lokasiGejala << ","
                 << durasiGejala << ","
                 << intensitasGejala << endl;
            cout << "Pasien berhasil diregistrasi." << endl;
            jumlahPasien++;
        }
        else
        {
            cout << "Gagal membuka file." << endl;
        }
        file.close();
    }
    else
    {
        cout << "Database pasien sudah penuh." << endl;
    }
}

bool login(int kesempatan = 3)
{

    if (jumlahPasien == 0)
    {
        cout << "List Pasien Kosong" << endl;
        return false;
    }

    cout << "===========================" << endl;
    cout << "|          Login          |" << endl;
    cout << "===========================" << endl;
    cout << "Username (Nama Pasien): ";
    string username;
    cin.ignore();
    getline(cin, username);

    cout << "Password (Tanggal Lahir, dd/mm/yyyy): ";
    string password;
    cin >> password;

    bool found = false;
    for (int i = 0; i < jumlahPasien; ++i)
    {
        if (databasePasien[i].nama == username && databasePasien[i].tanggalLahir == password)
        {
            found = true;
            clearTerminal();
            cout << "=============================================" << endl;
            cout << "Informasi Pasien:" << endl;
            cout << "Nama: " << databasePasien[i].nama << endl;
            cout << "Tanggal Lahir: " << databasePasien[i].tanggalLahir << endl;
            cout << "Asuransi BPJS: " << (databasePasien[i].asuransiBPJS ? "Ya" : "Tidak") << endl;
            cout << "Deskripsi Keluhan:" << endl;
            cout << "Jenis Gejala: " << databasePasien[i].jenisGejala << endl;
            cout << "Lokasi Gejala: " << databasePasien[i].lokasiGejala << endl;
            cout << "Durasi Gejala: " << databasePasien[i].durasiGejala << " hari" << endl;
            cout << "Intensitas Gejala: " << databasePasien[i].intensitasGejala << endl;
            return true;
        }
    }

    if (!found)
    {
        if (kesempatan > 1)
        {
            cout << "Data pasien tidak ditemukan. Silakan coba lagi." << endl;
            cout << "Kesempatan input tersisa: " << kesempatan - 1 << endl;
            return login(kesempatan - 1);
        }
        else
        {
            cout << "Kesempatan telah habis" << endl;
            return false;
        }
    }

    return false;
}
// int PEMBATASADMIN = 0;

void dataPasien()
{
    if (jumlahPasien == 0)
    {
        cout << "List Pasien Kosong" << endl;
        return;
    }

    while (true)
    {
        cout << "Daftar Pasien:" << endl;
        for (int i = 0; i < jumlahPasien; ++i)
        {
            cout << i + 1 << ". " << databasePasien[i].nama << endl;
        }

        int nomorPasien;
        while (true)
        {
            cout << "Masukkan nomor pasien yang ingin dikelola datanya (0 untuk kembali): ";
            cin >> nomorPasien;

            if (nomorPasien == 0)
            {
                clearTerminal();
                return;
            }

            else if (cin.fail() || cin.peek() != '\n')
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Input tidak valid. Silakan coba lagi." << endl;
            }
            else if (nomorPasien < 1 || nomorPasien > jumlahPasien)
            {
                cout << "Nomor pasien tidak valid. Silakan coba lagi." << endl;
            }
            else
            {
                --nomorPasien;
                break;
            }
        }

        int pilihan;
        while (true)
        {
            cout << "==============================" << endl;
            cout << "Menu Kelola Data Pasien untuk " << databasePasien[nomorPasien].nama << ":" << endl;
            cout << "==============================" << endl;
            cout << "1. Menampilkan Informasi Pasien" << endl;
            cout << "2. Mengubah data pasien" << endl;
            cout << "3. Kembali ke Menu Admin" << endl;
            cout << "Pilihan Anda: ";
            cin >> pilihan;
            if (cin.fail() || cin.peek() != '\n')
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                clearTerminal();
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            }
            else if (pilihan >= 1 && pilihan <= 3)
            {
                break;
            }
            else
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            }
        }

        switch (pilihan)
        {
        case 1:
            clearTerminal();
            cout << "=============================================" << endl;
            cout << "Informasi Pasien untuk " << databasePasien[nomorPasien].nama << ":" << endl;
            cout << "Nama: " << databasePasien[nomorPasien].nama << endl;
            cout << "Tanggal Lahir: " << databasePasien[nomorPasien].tanggalLahir << endl;
            cout << "Asuransi BPJS: " << (databasePasien[nomorPasien].asuransiBPJS ? "Ya" : "Tidak") << endl;
            cout << "Deskripsi Keluhan" << endl;
            cout << "Jenis Gejala: " << databasePasien[nomorPasien].jenisGejala << endl;
            cout << "Lokasi Gejala: " << databasePasien[nomorPasien].lokasiGejala << endl;
            cout << "Durasi Gejala: " << databasePasien[nomorPasien].durasiGejala << " hari" << endl;
            cout << "Intensitas Gejala: " << databasePasien[nomorPasien].intensitasGejala << endl;
            cout << "=============================================" << endl;
            break;

        case 2:
            clearTerminal();
            int pilihanUbah;
            while (true)
            {
                cout << "Pilih data yang ingin diubah untuk " << databasePasien[nomorPasien].nama << ":" << endl;
                cout << "1. Nama" << endl;
                cout << "2. Tanggal Lahir" << endl;
                cout << "3. Asuransi BPJS" << endl;
                cout << "4. Deskripsi Keluhan" << endl;
                cout << "5. Kembali ke Menu Admin" << endl;
                cout << "Pilihan Anda: ";
                cin >> pilihanUbah;

                if (cin.fail() || pilihanUbah < 1 || pilihanUbah > 5 || cin.peek() != '\n')
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    clearTerminal();
                    cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                }
                else
                {
                    switch (pilihanUbah)
                    {
                    case 1:
                    {
                        cout << "Masukkan nama baru: ";
                        cin.ignore();
                        string nama;
                        while (true)
                        {
                            getline(cin, nama);
                            if (nama.empty())
                            {
                                cout << "Nama lengkap tidak boleh kosong." << endl;
                            }
                            else if (!isValidString(nama))
                            {
                                cout << "Nama lengkap tidak boleh mengandung angka atau simbol." << endl;
                            }
                            else
                            {
                                databasePasien[nomorPasien].nama = nama;
                                cout << "Nama berhasil diubah." << endl;
                                break;
                            }
                        }
                        break;
                    }

                    case 2:
                    {
                        cout << "Masukkan tanggal lahir baru: ";
                        cin.ignore();
                        string tanggalLahir;
                        while (true)
                        {
                            getline(cin, tanggalLahir);
                            if (!isValidDate(tanggalLahir))
                            {
                                cout << "Tanggal lahir tidak valid. Format yang benar adalah dd/mm/yyyy." << endl;
                            }
                            else
                            {
                                databasePasien[nomorPasien].tanggalLahir = tanggalLahir;
                                cout << "Tanggal lahir berhasil diubah." << endl;
                                break;
                            }
                        }
                        break;
                    }

                    case 3:
                    {
                        cout << "Apakah pasien memiliki asuransi BPJS? (1 = Ya, 0 = Tidak): ";
                        int asuransiBPJS;
                        while (true)
                        {
                            cin >> asuransiBPJS;
                            if (asuransiBPJS == 1)
                            {
                                databasePasien[nomorPasien].asuransiBPJS = true;
                                cout << "Status asuransi BPJS berhasil diubah." << endl;
                                break;
                            }
                            else if (asuransiBPJS == 0)
                            {
                                databasePasien[nomorPasien].asuransiBPJS = false;
                                cout << "Status asuransi BPJS berhasil diubah." << endl;
                                break;
                            }
                            else
                            {
                                cout << "Input tidak valid untuk Asuransi BPJS. Harus '1' atau '0'." << endl;
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            }
                        }
                        break;
                    }

                    case 4:
                    {
                        cout << "Masukkan deskripsi keluhan baru" << endl;

                        string jenisGejala;
                        cout << "Jenis Gejala: ";
                        cin.ignore();
                        while (true)
                        {
                            getline(cin, jenisGejala);
                            if (jenisGejala.empty())
                            {
                                cout << "Jenis gejala tidak boleh kosong." << endl;
                            }
                            else if (!isValidString(jenisGejala))
                            {
                                cout << "Jenis gejala tidak boleh mengandung angka atau simbol." << endl;
                            }
                            else
                            {
                                databasePasien[nomorPasien].jenisGejala = jenisGejala;
                                break;
                            }
                        }

                        string lokasiGejala;
                        cout << "Lokasi Gejala: ";
                        while (true)
                        {
                            getline(cin, lokasiGejala);
                            if (lokasiGejala.empty())
                            {
                                cout << "Lokasi gejala tidak boleh kosong." << endl;
                            }
                            else if (!isValidString(lokasiGejala))
                            {
                                cout << "Lokasi gejala tidak boleh mengandung angka atau simbol." << endl;
                            }
                            else
                            {
                                databasePasien[nomorPasien].lokasiGejala = lokasiGejala;
                                break;
                            }
                        }

                        string durasiGejala;
                        cout << "Durasi Gejala (hari): ";
                        while (true)
                        {
                            getline(cin, durasiGejala);
                            if (durasiGejala.empty())
                            {
                                cout << "Durasi gejala tidak boleh kosong." << endl;
                            }
                            else if (!isValidNumber(durasiGejala))
                            {
                                cout << "Durasi gejala tidak boleh mengandung string atau simbol." << endl;
                                cout << "Durasi Gejala (hari): ";
                            }
                            else
                            {
                                int durasi = stoi(durasiGejala);
                                if (durasi < 1)
                                {
                                    cout << "Durasi gejala harus lebih dari 0 hari." << endl;
                                    cout << "Durasi Gejala (hari): ";
                                }
                                else
                                {
                                    databasePasien[nomorPasien].durasiGejala = durasiGejala;
                                    break;
                                }
                            }
                        }

                        string intensitasGejala;
                        cout << "Intensitas Gejala (ringan, sedang, berat): ";
                        while (true)
                        {
                            getline(cin, intensitasGejala);
                            if (intensitasGejala != "ringan" && intensitasGejala != "sedang" && intensitasGejala != "berat")
                            {
                                cout << "Intensitas gejala harus ringan, sedang, atau berat." << endl;
                            }
                            else if (intensitasGejala.empty())
                            {
                                cout << "Intensitas gejala tidak boleh kosong." << endl;
                            }
                            else if (!isValidString(intensitasGejala))
                            {
                                cout << "Intensitas gejala tidak boleh mengandung angka atau simbol." << endl;
                            }
                            else
                            {
                                databasePasien[nomorPasien].intensitasGejala = intensitasGejala;
                                break;
                            }
                        }
                        cout << "Deskripsi keluhan berhasil diubah." << endl;
                        break;
                    }

                    case 5:
                        return;
                    }
                    break;
                }
            }
            break;
        case 3:
            return;
        default:
            cout << "Pilihan tidak valid." << endl;
            break;
        }
    }
}

void dataDokterSpesialis()
{
    string inputSpesialis;
    cin.ignore();
    cout << "Kelola Data Dokter" << endl;
    while (true)
    {
        cout << "Masukkan spesialisasi dokter yang akan ditambahkan: ";
        getline(cin, inputSpesialis);
        if (inputSpesialis.empty())
        {
            cout << "Spesialis tidak boleh kosong." << endl;
            continue;
        }
        else if (!isValidString(inputSpesialis))
        {
            cout << "Spesialis tidak boleh mengandung angka atau simbol." << endl;
            continue;
        }
        else
        {
            break;
        }
    }

    bool sudahAda = false;
    for (int i = 0; i < jumlahSpesialis; ++i)
    {
        transform(inputSpesialis.begin(), inputSpesialis.end(), inputSpesialis.begin(), ::tolower);
        if (spesialisDokter[i] == inputSpesialis)
        {
            sudahAda = true;
            break;
        }
    }

    if (!sudahAda)
    {
        if (jumlahSpesialis < MAX_DOKTER)
        {
            spesialisDokter[jumlahSpesialis] = inputSpesialis;
            jumlahSpesialis++;
            cout << "Spesialisasi berhasil ditambahkan." << endl;
        }
        else
        {
            cout << "Array spesialis sudah penuh." << endl;
        }
    }
    else
    {
        cout << "Spesialisasi sudah ada dalam array." << endl;
    }
}

void semuaDataPasien()
{
    if (jumlahPasien == 0)
    {
        cout << "Daftar Pasien Kosong" << endl;
        return;
    }
    cout << "Data Semua Pasien:" << endl;
    for (int i = 0; i < jumlahPasien; ++i)
    {
        cout << "Nama: " << databasePasien[i].nama << endl;
        cout << "Tanggal Lahir: " << databasePasien[i].tanggalLahir << endl;
        cout << "Asuransi BPJS: " << (databasePasien[i].asuransiBPJS ? "Ya" : "Tidak") << endl;
        cout << "Jenis Gejala: " << databasePasien[i].jenisGejala << endl;
        cout << "Lokasi Gejala: " << databasePasien[i].lokasiGejala << endl;
        cout << "Durasi Gejala: " << databasePasien[i].durasiGejala << " hari" << endl;
        cout << "Intensitas Gejala: " << databasePasien[i].intensitasGejala << endl;
        cout << "Jenis Penyakit: " << databasePasien[i].jenisPenyakit << endl;
        cout << "Obat: " << databasePasien[i].obat << endl;
        cout << "Jumlah Kontrol: " << databasePasien[i].jumlahKontrol << endl;
        cout << "---------------------------------" << endl;
    }
}

void semuaDataDokterSpesialis()
{
    cout << "=============================" << endl;
    cout << "||  List Dokter Spesialis  ||" << endl;
    cout << "=============================" << endl;
    for (int i = 0; i < jumlahSpesialis; ++i)
    {
        cout << i + 1 << ". " << spesialisDokter[i] << endl;
    }
}

void sorting(Pasien *databasePasien, int jumlahPasien)
{
    for (int i = 0; i < jumlahPasien - 1; ++i)
    {
        for (int j = 0; j < jumlahPasien - i - 1; ++j)
        {
            if (databasePasien[j].nama > databasePasien[j + 1].nama)
            {
                swap(databasePasien[j], databasePasien[j + 1]);
            }
        }
    }
    cout << "Data pasien berhasil diurutkan berdasarkan nama." << endl;
    return;
}

void searchingPasienNama()
{
    if (jumlahPasien == 0)
    {
        cout << "List Pasien Kosong" << endl;
        return;
    }
    cout << "Masukkan nama pasien yang ingin dicari: ";
    string nama;
    cin.ignore();
    getline(cin, nama);

    bool found = false;
    for (int i = 0; i < jumlahPasien; ++i)
    {
        if (databasePasien[i].nama == nama)
        {
            found = true;
            cout << "Data Pasien Ditemukan:" << endl;
            cout << "Nama: " << databasePasien[i].nama << endl;
            cout << "Tanggal Lahir: " << databasePasien[i].tanggalLahir << endl;
            cout << "Asuransi BPJS: " << (databasePasien[i].asuransiBPJS ? "Ya" : "Tidak") << endl;
            cout << "Jenis Gejala: " << databasePasien[i].jenisGejala << endl;
            cout << "Lokasi Gejala: " << databasePasien[i].lokasiGejala << endl;
            cout << "Durasi Gejala: " << databasePasien[i].durasiGejala << endl;
            cout << "Intensitas Gejala: " << databasePasien[i].intensitasGejala << endl;
            cout << "Jenis Penyakit: " << databasePasien[i].jenisPenyakit << endl;
            cout << "Obat: " << databasePasien[i].obat << endl;
            cout << "Jumlah Kontrol: " << databasePasien[i].jumlahKontrol << endl;
            cout << "---------------------------------" << endl;
            break;
        }
    }

    if (!found)
    {
        cout << "Data pasien tidak ditemukan." << endl;
    }
}

void searchingPasienBPJS()
{
    if (jumlahPasien == 0)
    {
        cout << "List Pasien Kosong" << endl;
        return;
    }
    cout << "Daftar Pasien dengan Asuransi BPJS:" << endl;
    bool found = false;
    for (int i = 0; i < jumlahPasien; ++i)
    {
        if (databasePasien[i].asuransiBPJS)
        {
            found = true;
            cout << "Nama: " << databasePasien[i].nama << endl;
            cout << "Tanggal Lahir: " << databasePasien[i].tanggalLahir << endl;
            cout << "---------------------------------" << endl;
        }
    }

    if (!found)
    {
        cout << "Tidak ada pasien dengan asuransi BPJS yang ditemukan." << endl;
    }
}

void searching()
{
    bool keluar = false;
    while (!keluar)
    {
        int pilihan;
        cout << "Menu Searching" << endl;
        cout << "1. Cari Pasien Berdasarkan Nama" << endl;
        cout << "2. Cari Pasien Berdasarkan BPJS" << endl;
        cout << "3. Keluar" << endl;
        cout << "Masukkan pilihan anda: ";

        cin >> pilihan;
        if (cin.fail() || cin.peek() != '\n')
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearTerminal();
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
        else
        {
            switch (pilihan)
            {
            case 1:
                clearTerminal();
                searchingPasienNama();
                break;
            case 2:
                clearTerminal();
                searchingPasienBPJS();
                break;
            case 3:
                keluar = true;
                break;
            default:
                clearTerminal();
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                break;
            }
        }
    }
}

void historiRegistrasi()
{

    cout << "Mengunduh ... ";
    loadingAnimasi();

    ifstream file("a-belumbayar.csv");

    if (file.is_open())
    {
        string line;
        getline(file, line);

        while (getline(file, line))
        {
            stringstream ss(line);
            string nama, tanggalLahir, asuransiBPJSStr, jenisGejala, lokasiGejala, durasiGejala, intensitasGejala;

            getline(ss, nama, ',');
            getline(ss, tanggalLahir, ',');
            getline(ss, asuransiBPJSStr, ',');
            getline(ss, jenisGejala, ',');
            getline(ss, lokasiGejala, ',');
            getline(ss, durasiGejala, ',');
            getline(ss, intensitasGejala, ',');

            bool asuransiBPJS = (asuransiBPJSStr == "true");

            databasePasien[jumlahPasien].nama = nama;
            databasePasien[jumlahPasien].tanggalLahir = tanggalLahir;
            databasePasien[jumlahPasien].asuransiBPJS = asuransiBPJS;
            databasePasien[jumlahPasien].jenisGejala = jenisGejala;
            databasePasien[jumlahPasien].lokasiGejala = lokasiGejala;
            databasePasien[jumlahPasien].durasiGejala = durasiGejala;
            databasePasien[jumlahPasien].intensitasGejala = intensitasGejala;

            jumlahPasien++;
        }
        file.close();
        cout << "Berhasil mengunduh Data Registrasi" << endl;
    }
    else
    {
        cout << "Gagal membuka file CSV." << endl;
    }
}

void historiPembayaran()
{
    cout << "Mengunduh ...";
    loadingAnimasi();

    ifstream file("b-sudahbayar.csv");

    if (file.is_open())
    {
        string line;
        getline(file, line);

        while (getline(file, line))
        {
            stringstream ss(line);
            string nama, jenisPenyakit, obat;
            int jumlahKontrol, tagihanKontrol, tagihanObat, totalBayar;

            getline(ss, nama, ',');
            getline(ss, jenisPenyakit, ',');
            getline(ss, obat, ',');
            ss >> jumlahKontrol;
            ss.ignore();
            ss >> tagihanKontrol;
            ss.ignore();
            ss >> tagihanObat;
            ss.ignore();
            ss >> totalBayar;

            cout << "Nama: " << nama << endl;
            cout << "Jenis Penyakit: " << jenisPenyakit << endl;
            cout << "Banyak Obat: " << obat << endl;
            cout << "Jumlah Kontrol: " << jumlahKontrol << endl;
            cout << "Tagihan Kontrol: " << tagihanKontrol << endl;
            cout << "Tagihan Obat: " << tagihanObat << endl;
            cout << "Total Bayar: " << totalBayar << endl;
            cout << endl;
        }

        file.close();
        cout << "Berhasil mengunduh Histori Pembayaran" << endl;
    }
    else
    {
        cout << "Gagal membuka file histori pembayaran." << endl;
    }
}

// int PEMBATASDOKTER = 0;

void eksekusiSpesialis(string spesialis)
{

    cout << "Melakukan pemeriksaan oleh dokter spesialis " << spesialis << " ";

    loadingAnimasi();

    cout << "Memberikan rekomendasi pengobatan dari dokter spesialis " << spesialis << " ";
    loadingAnimasi();

    int nomorPasien = jumlahPasien - 1;
    int jumlahobat;

    while (true)
    {
        cout << "Masukkan berapa banyak obat yang harus diberikan: ";
        cin >> jumlahobat;
        if (cin.fail() || cin.peek() != '\n')
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Harap masukkan angka." << endl;
        }
        else if (jumlahobat < 1)
        {
            cout << "Jumlah obat tidak boleh negatif." << endl;
        }
        else
        {
            databasePasien[nomorPasien].obat = jumlahobat;
            break;
        }
    }
}

void dokterSpesialis()
{
    cout << "Dokter Spesialis yang Tersedia:" << endl;

    for (int i = 0; i < jumlahSpesialis; ++i)
    {
        cout << i + 1 << ". " << spesialisDokter[i] << endl;
    }

    string pilihanSpesialis;

    while (true)
    {
        cout << "Pilih jenis spesialis yang sesuai dengan kondisi pasien (X untuk batalkan rujukan): ";
        cin >> pilihanSpesialis;

        if (pilihanSpesialis == "x" || pilihanSpesialis == "X")
        {
            cout << "Anda membatalkan rujukan ke spesialis" << endl;
            return;
        }

        transform(pilihanSpesialis.begin(), pilihanSpesialis.end(), pilihanSpesialis.begin(), ::tolower);

        bool validInput = false;
        for (int i = 0; i < jumlahSpesialis; ++i)
        {
            if (pilihanSpesialis == spesialisDokter[i])
            {
                validInput = true;
                break;
            }
        }

        if (validInput)
        {
            break;
        }
        else
        {
            cout << "Input tidak valid. Masukkan pilihan yang sesuai." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    cout << "Pasien akan dirujuk ke dokter spesialis " << pilihanSpesialis << "." << endl;
    eksekusiSpesialis(pilihanSpesialis);
}

void eksekusi()
{
    while (true)
    {
        if (jumlahPasien == 0)
        {
            cout << "List Pasien Kosong" << endl;
            return;
        }

        cout << "Daftar Pasien yang Akan Diperiksa:" << endl;

        for (int i = 0; i < jumlahPasien; ++i)
        {
            cout << i + 1 << ". " << databasePasien[i].nama << endl;
        }

        int nomorPasien;
        cout << "Masukkan nomor pasien yang akan diperiksa (0 untuk kembali): ";
        cin >> nomorPasien;

        if (nomorPasien == 0)
        {
            return;
        }

        if (cin.fail() || cin.peek() != '\n')
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearTerminal();
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            continue;
        }

        if (nomorPasien < 1 || nomorPasien > jumlahPasien)
        {
            cout << "Nomor pasien tidak valid. Masukkan nomor dari 1 sampai " << jumlahPasien << "." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (nomorPasien > 0 && nomorPasien <= jumlahPasien)
        {
            --nomorPasien;
            cout << "Informasi Pasien yang Akan Diperiksa:" << endl;
            cout << "Nama: " << databasePasien[nomorPasien].nama << endl;
            cout << "Tanggal Lahir: " << databasePasien[nomorPasien].tanggalLahir << endl;
            cout << "Deskripsi Keluhan:" << endl;
            cout << "Jenis Gejala: " << databasePasien[nomorPasien].jenisGejala << endl;
            cout << "Lokasi Gejala: " << databasePasien[nomorPasien].lokasiGejala << endl;
            cout << "Durasi Gejala: " << databasePasien[nomorPasien].durasiGejala << " hari" << endl;
            cout << "Intensitas Gejala: " << databasePasien[nomorPasien].intensitasGejala << endl;
            while (true)
            {
                cout << "Masukkan jenis penyakit: ";
                cin.ignore();
                string jenispenyakit;
                getline(cin, jenispenyakit);
                if (jenispenyakit.empty())
                {
                    cout << "Jenis penyakit tidak boleh kosong." << endl;
                }
                else if (!isValidString(jenispenyakit))
                {
                    cout << "Jenis penyakit tidak boleh mengandung angka" << endl;
                }
                else
                {
                    databasePasien[nomorPasien].jenisPenyakit = jenispenyakit;
                    cout << "Jenis penyakit berhasil ditambahkan." << endl;
                    break;
                }
            }

            int jumlahobat;
            while (true)
            {
                cout << "Masukkan berapa banyak obat yang harus diberikan: ";
                cin >> jumlahobat;
                if (cin.fail() || cin.peek() != '\n')
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Input tidak valid. Harap masukkan angka." << endl;
                }
                else if (jumlahobat < 1)
                {
                    cout << "Jumlah obat tidak boleh negatif atau 0." << endl;
                    continue;
                }
                else
                {
                    databasePasien[nomorPasien].obat = jumlahobat;
                    break;
                }
            }

            string pilihan;
            while (true)
            {
                cout << "Apakah pasien perlu dirujuk ke dokter spesialis? (ya/tidak): ";
                cin >> pilihan;
                if (pilihan == "ya" || pilihan == "Ya" || pilihan == "YA")
                {
                    cout << "Pasien akan dirujuk ke dokter spesialis." << endl;
                    dokterSpesialis();
                    break;
                }
                else if (pilihan == "tidak" || pilihan == "Tidak" || pilihan == "TIDAK")
                {
                    cout << "Pasien tidak perlu dirujuk ke dokter spesialis." << endl;
                    break;
                }
                else
                {
                    cout << "Input tidak valid. Masukkan ya atau tidak." << endl;
                }
            }
            break;
        }
        else
        {
            cout << "Nomor pasien tidak valid." << endl;
        }
    }
}

void kontrolPasien()
{
    while (true)
    {
        if (jumlahPasien == 0)
        {
            cout << "List Pasien Kosong" << endl;
            return;
        }

        cout << "Daftar Pasien yang Akan Dijadwalkan Kontrol:" << endl;
        for (int i = 0; i < jumlahPasien; ++i)
        {
            cout << i + 1 << ". " << databasePasien[i].nama << endl;
        }

        int nomorPasien;
        cout << "Masukkan nomor pasien yang akan dijadwalkan kontrol (0 untuk kembali): ";
        cin >> nomorPasien;

        if (nomorPasien == 0)
        {
            return;
        }

        if (cin.fail() || cin.peek() != '\n')
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearTerminal();
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            continue;
        }

        if (nomorPasien < 1 || nomorPasien > jumlahPasien)
        {
            cout << "Nomor pasien tidak valid. Masukkan nomor dari 1 sampai " << jumlahPasien << "." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (nomorPasien > 0 && nomorPasien <= jumlahPasien)
        {
            --nomorPasien;
            cout << "Informasi Pasien yang Akan Dijadwalkan Kontrol:" << endl;
            cout << "Nama: " << databasePasien[nomorPasien].nama << endl;
            cout << "Tanggal Lahir: " << databasePasien[nomorPasien].tanggalLahir << endl;
            cout << "Deskripsi Keluhan:" << endl;
            cout << "Jenis Gejala: " << databasePasien[nomorPasien].jenisGejala << endl;
            cout << "Lokasi Gejala: " << databasePasien[nomorPasien].lokasiGejala << endl;
            cout << "Durasi Gejala: " << databasePasien[nomorPasien].durasiGejala << " hari" << endl;
            cout << "Intensitas Gejala: " << databasePasien[nomorPasien].intensitasGejala << endl;
            cout << "Jenis Penyakit: " << databasePasien[nomorPasien].jenisPenyakit << endl;
            int jumlahKontrol;
            cout << "Masukkan jumlah kontrol yang diperlukan oleh pasien: ";
            cin >> jumlahKontrol;
            if (cin.fail() || cin.peek() != '\n')
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Input tidak valid. Harap masukkan angka." << endl;
            }
            else if (jumlahKontrol < 1)
            {
                cout << "Jumlah kontrol tidak boleh negatif atau 0." << endl;
            }
            else
            {
                databasePasien[nomorPasien].jumlahKontrol = jumlahKontrol;
                cout << "Jumlah kontrol berhasil dijadwalkan untuk pasien tersebut." << endl;
                break;
            }
        }
    }
}
// int PEMBATASMENU = 0;

void pasien()
{
    bool keluar = false;
    while (!keluar)
    {
        int pilihan;
        cout << "=============================" << endl;
        cout << "|        Menu Pasien        |" << endl;
        cout << "=============================" << endl;
        cout << "| 1. Registrasi             |" << endl;
        cout << "| 2. Login                  |" << endl;
        cout << "| 3. Keluar                 |" << endl;
        cout << "=============================" << endl;
        cout << "Masukkan pilihan anda: ";
        cin >> pilihan;

        if (cin.fail() || cin.peek() != '\n')
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearTerminal();
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            continue;
        }

        switch (pilihan)
        {
        case 1:
            clearTerminal();
            registrasi();
            break;
        case 2:
            clearTerminal();
            login();
            break;
        case 3:
            clearTerminal();
            keluar = true;
            break;
        default:
            clearTerminal();
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            break;
        }
    }
}

void admin()
{
    bool keluar = false;
    while (!keluar)
    {
        int pilihan;
        cout << "=============================================" << endl;
        cout << "|                 Menu Admin                |" << endl;
        cout << "=============================================" << endl;
        cout << "|  1. Kelola Data Pasien                    |" << endl;
        cout << "|  2. Tambah Dokter Spesialis               |" << endl;
        cout << "|  3. Sorting Pasien Berdasarkan ASCII      |" << endl;
        cout << "|  4. Searching Pasien                      |" << endl;
        cout << "|     (Berdasarkan Nama atau BPJS )         |" << endl;
        cout << "|  5. Tampilkan List Data Pasien            |" << endl;
        cout << "|  6. Tampilkan List Spesialis Dokter       |" << endl;
        cout << "|  7. Histori Registrasi                    |" << endl;
        cout << "|  8. Histori Pembayaran Pasien             |" << endl;
        cout << "|  9. Keluar                                |" << endl;
        cout << "=============================================" << endl;
        cout << "Masukkan pilihan anda: ";

        cin >> pilihan;

        if (cin.fail() || cin.peek() != '\n')
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearTerminal();
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            continue;
        }

        switch (pilihan)
        {
        case 1:
            clearTerminal();
            dataPasien();
            break;
        case 2:
            clearTerminal();
            dataDokterSpesialis();
            break;
        case 3:
            clearTerminal();
            sorting(&databasePasien[0], jumlahPasien);
            break;
        case 4:
            clearTerminal();
            searching();
            break;
        case 5:
            clearTerminal();
            semuaDataPasien();
            break;
        case 6:
            clearTerminal();
            semuaDataDokterSpesialis();
            break;
        case 7:
            clearTerminal();
            historiRegistrasi();
            break;
        case 8:
            clearTerminal();
            historiPembayaran();
            break;
        case 9:
            clearTerminal();
            keluar = true;
            break;
        default:
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            break;
        }
    }
}

void dokter()
{
    bool keluar = false;
    while (!keluar)
    {
        int pilihan;
        cout << "===============================" << endl;
        cout << "|         Menu Dokter         |" << endl;
        cout << "===============================" << endl;
        cout << "| 1. Proses Pemeriksaan Pasien|" << endl;
        cout << "| 2. Penjadwalan Kontrol      |" << endl;
        cout << "| 3. Keluar                   |" << endl;
        cout << "===============================" << endl;
        cout << "Masukkan pilihan anda: ";

        cin >> pilihan;

        if (cin.fail() || cin.peek() != '\n')
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearTerminal();
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            continue;
        }

        switch (pilihan)
        {
        case 1:
            clearTerminal();
            eksekusi();
            break;
        case 2:
            clearTerminal();
            kontrolPasien();
            break;
        case 3:
            clearTerminal();
            keluar = true;
            break;
        default:
            clearTerminal();
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            break;
        }
    }
}

void pembayaran()
{
    while (true)
    {
        if (jumlahPasien == 0)
        {
            cout << "List Pasien Kosong" << endl;
            return;
        }

        cout << "Daftar Pasien:" << endl;
        for (int i = 0; i < jumlahPasien; ++i)
        {
            cout << i + 1 << ". " << databasePasien[i].nama << endl;
        }

        int nomorPasien;
        cout << "Masukkan nomor pasien yang ingin dibayarkan tagihannya (0 untuk keluar): ";
        cin >> nomorPasien;

        if (cin.fail() || cin.peek() != '\n')
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearTerminal();
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            continue;
        }

        if (nomorPasien == 0)
        {
            cout << "Anda keluar " << endl;
            break;
        }

        if (nomorPasien > 0 && nomorPasien <= jumlahPasien)
        {
            --nomorPasien;
            int tagihanKontrol = databasePasien[nomorPasien].jumlahKontrol * HARGA_KONTROL;
            int tagihanObat = databasePasien[nomorPasien].obat * HARGA_OBAT;
            int totalTagihan = tagihanKontrol + tagihanObat;

            if (databasePasien[nomorPasien].asuransiBPJS)
            {
                totalTagihan *= 0.9;
            }

            if (totalTagihan == 0)
            {
                cout << "Pasien ini belum memiliki tagihan" << endl;
                break;
            }

            cout << "Tagihan untuk " << databasePasien[nomorPasien].nama << ":" << endl;
            cout << "Jenis Penyakit: " << databasePasien[nomorPasien].jenisPenyakit << endl;
            cout << "Jumlah Kontrol: " << databasePasien[nomorPasien].jumlahKontrol << " x " << HARGA_KONTROL << " = " << tagihanKontrol << endl;
            cout << "Jumlah Obat: " << databasePasien[nomorPasien].obat << " x " << HARGA_OBAT << " = " << tagihanObat << endl;
            cout << "Total Tagihan: " << totalTagihan << endl;

            while (true)
            {
                double uangDibayar;
                while (true)
                {
                    cout << "Masukkan jumlah uang yang dibayarkan: ";
                    cin >> uangDibayar;
                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Input tidak valid. Masukkan angka yang benar." << endl;
                    }
                    else if (uangDibayar < 0)
                    {
                        cout << "Jumlah uang yang dibayarkan tidak boleh negatif." << endl;
                    }
                    else
                    {
                        break;
                    }
                }
                if (uangDibayar >= totalTagihan)
                {
                    double kembalian = uangDibayar - totalTagihan;
                    cout << "Pembayaran berhasil." << endl;
                    cout << "Kembalian: " << kembalian << endl;

                    ofstream file("b-sudahbayar.csv", ios::app);
                    if (file.is_open())
                    {
                        file << databasePasien[nomorPasien].nama << ","
                             << databasePasien[nomorPasien].jenisPenyakit << ","
                             << databasePasien[nomorPasien].obat << ","
                             << databasePasien[nomorPasien].jumlahKontrol << ","
                             << tagihanKontrol << ","
                             << tagihanObat << ","
                             << totalTagihan << endl;
                        file.close();
                        cout << "Data pembayaran berhasil disimpan" << endl;
                    }
                    else
                    {
                        cout << "Gagal membuka file" << endl;
                    }

                    for (int i = nomorPasien; i < jumlahPasien - 1; ++i)
                    {
                        databasePasien[i] = databasePasien[i + 1];
                    }
                    --jumlahPasien;
                    break;
                }
                else
                {
                    cout << "Uang yang dibayarkan kurang dari total tagihan. Silakan masukkan kembali." << endl;
                }
            }
            break;
        }
        else
        {
            cout << "Nomor pasien tidak valid." << endl;
            break;
        }
    }
}

int main()
{
    int pilihan;
    do
    {
        cout << "===============================" << endl;
        cout << "|           Menu Utama        |" << endl;
        cout << "===============================" << endl;
        cout << "|  1. Pasien                  |" << endl;
        cout << "|  2. Admin                   |" << endl;
        cout << "|  3. Dokter                  |" << endl;
        cout << "|  4. Loket Pembayaran        |" << endl;
        cout << "|  5. Keluar                  |" << endl;
        cout << "===============================" << endl;
        cout << "Pilihan Anda: ";

        while (!(cin >> pilihan) || cin.peek() != '\n')
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Silakan masukkan angka dari 1 sampai 5." << endl;
            cout << "Pilihan Anda: ";
        }

        switch (pilihan)
        {
        case 1:
            clearTerminal();
            pasien();
            break;
        case 2:
            clearTerminal();
            admin();
            break;
        case 3:
            clearTerminal();
            dokter();
            break;
        case 4:
            clearTerminal();
            pembayaran();
            break;
        case 5:
            clearTerminal();
            cout << "Terima kasih!" << endl;
            return 0;
        default:
            clearTerminal();
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            break;
        }
    } while (pilihan != 0);
    return 0;
}