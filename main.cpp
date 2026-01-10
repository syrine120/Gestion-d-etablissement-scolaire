#include "Exceptions.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;
void appendJson(const string& filename, const string& jsonObject) {
    ofstream f(filename, ios::app);
    if (!f) {
        cout << "Erreur ouverture fichier " << filename << endl;
        return;
    }
    f << jsonObject << ",\n";
    f.close();
}
class personne {
protected:
    string nom;
    string prenom;
    int id;

public:
    personne(string n = "", string p = "", int i = 0)
        : nom(n), prenom(p), id(i) {}
    virtual ~personne() {} 
    string getNom() const { 
        return nom; 
    }
    string getPrenom() const { 
        return prenom; 
    }
    int getId() const { 
        return id; 
    }

    void setNom(const string &n) { 
        nom = n; 
    }
    void setPrenom(const string &p) { 
        prenom = p; 
    }
    void setId(int i) { 
        id = i; 
    }
    void remplir_info() {
       cout << "Nom : ";
      getline(cin, nom);

       cout << "Prenom : ";
       getline(cin, prenom);

       cout << "ID : ";
       cin >> id;

    }

    virtual void afficher() const = 0;
   bool compareId(const personne& p) const{
     return p.getId()== this->id;
   }
};
class GestionPersonnes {
    vector<shared_ptr<personne>> personnes;  

public:
    void ajouterPersonne(shared_ptr<personne> p) {
        for (auto& existing : personnes) {
            if (existing->getId() == p->getId()) {
                cout << "ID existe deja !\n";
                return;
            }
        }
        personnes.push_back(p);
    }

    void afficherTous() const {
        for (auto& p : personnes) {
            p->afficher();  
            cout << "\n";
        }
    }
};


class Professeur : public personne {
public:
    Professeur(string n = "", string p = "", int i = 0)
        : personne(n, p, i) {}
    void setId(int i) {
     if (i <= 0) throw invalid_argument("ID doit etre positif !");
     id = i;
    }

    void afficher() const override {
        cout << "Professeur ID: " << id << ", Nom: " << nom << ", Prenom: " << prenom << endl;
    }

    void remplir_info() {
        cout << "Nom : ";
        getline(cin, nom);

        cout << "Prenom : ";
        getline(cin, prenom);

        cout << "ID : ";
        cin >> id;
        cin.ignore();
        setId(id);//utuliser pour valider l'id
    }
    json toJson() const {
        return json{{"id", id}, {"nom", nom}, {"prenom", prenom}};
    }

    static Professeur fromJson(const json& j) {
        return Professeur(j.at("nom").get<string>(), j.at("prenom").get<string>(), j.at("id").get<int>());
    }
};

// Classe GestionProf
class GestionProf {
private:
    vector<Professeur> profs;

public:
    void ajouterProfesseur(const Professeur& p) {
        for (const auto& prof : profs) {
            if (prof.compareId(p)) {
                cout << "Le professeur avec ID " << p.getId() << " existe deja.\n";
                return;
            }
        }
        profs.push_back(p);
        cout << "Professeur ajoute: " << p.getNom() << " (ID: " << p.getId() << ")\n";
    }

    void modifierProfesseur(int id) {
        for (auto& prof : profs) {
            if (prof.getId() == id) {
                int choix;
                string input;
                int newId;

                do {
                    cout << "\n--- Modifier Professeur (ID " << id << ") ---\n";
                    cout << "1. Modifier nom\n";
                    cout << "2. Modifier prenom\n";
                    cout << "3. Modifier ID\n";
                    cout << "0. Terminer\n";
                    cout << "Choix: ";
                    cin >> choix;
                    cin.ignore();

                    switch (choix) {
                        case 1:
                            cout << "Nouveau nom: ";
                            getline(cin, input);
                            prof.setNom(input);
                            break;

                        case 2:
                            cout << "Nouveau prenom: ";
                            getline(cin, input);
                            prof.setPrenom(input);
                            break;

                        case 3:
                            cout << "Nouvel ID: ";
                            cin >> newId;
                            cin.ignore();
                            prof.setId(newId); // validation incluse
                            break;

                        case 0:
                            cout << "Modification terminee.\n";
                            break;

                        default:
                            cout << "Choix invalide.\n";
                    }

                } while (choix != 0);

                cout << "Professeur modifie avec succes.\n";
                return;
            }
        }

        cout << "Aucun professeur trouve avec l'ID " << id << ".\n";
    }

        Professeur* rechercherParId(int id) {
        for (auto& prof : profs) {
            if (prof.getId() == id) return &prof;
        }
        return nullptr;
    }

    vector<Professeur*> rechercherParNom(const string& nom) {
        vector<Professeur*> result;
        for (auto& prof : profs) {
            if (prof.getNom() == nom) result.push_back(&prof);
        }
        return result;
    }
    void afficherTous() const {
        for (const auto& prof : profs) prof.afficher();
    }
    vector<Professeur>& getProfs() { 
        return profs; 
    }
    void supprimerProfesseur(int id) {
        auto it = remove_if(profs.begin(), profs.end(),
            [id](const Professeur& p) {
            return p.getId() == id;
        });

        if (it == profs.end()) {
            cout << "Professeur introuvable.\n";
         return;
     }

        profs.erase(it, profs.end());
        cout << "Professeur supprime.\n";
   }

};

class etudiant : public personne {
    private:
     string filiere;
     string niveau;
 public:
    etudiant(string n = "", string p = "", int i = 0,string f="", string niv="")
        : personne(n, p, i), filiere(f), niveau(niv) {}
    
    void setId(int i) {
    if (i <= 0) throw invalid_argument("ID doit etre positif !");
    id = i;
    }
    string getFiliere() const { return filiere; }
    string getNiveau() const { return niveau; }

    void setFiliere(const string& f) { filiere = f; }
    void setNiveau(const string& n) { niveau = n; }

       void remplir_info_etudiant() {
        cout << "Nom : ";
        getline(cin, nom);

        cout << "Prenom : ";
        getline(cin, prenom);

        cout << "ID : ";
        cin >> id;
        cin.ignore();
        setId(id); // utiliser pour valider l'id

        cout << "Filiere : ";
        getline(cin, filiere);

        cout << "Niveau : ";
        getline(cin, niveau);
    }

    void afficher() const override {
        cout << "ID: " << id
             << ", Nom: " << nom
             << ", Prenom: " << prenom
             << ", Filiere: " << filiere
             << ", Niveau: " << niveau << endl;
    }
    
    json toJson() const {
    return {
        {"id", id},
        {"nom", nom},
        {"prenom", prenom},
        {"filiere", filiere},
        {"niveau", niveau}
    };
   }


    static etudiant fromJson(const json& j) {
        return etudiant(
            j.at("nom").get<string>(),
            j.at("prenom").get<string>(),
            j.at("id").get<int>(),
            j.at("filiere").get<string>(),
            j.at("niveau").get<string>()
        );
    }
};
// After your 'etudiant' class definition ends
ostream& operator<<(ostream& os, const etudiant& e) {
    os << "ID: " << e.getId()
       << ", Nom: " << e.getNom()
       << ", Prenom: " << e.getPrenom()
       << ", Filiere: " << e.getFiliere()
       << ", Niveau: " << e.getNiveau();
    return os;
}

// Gestion des étudiants
class GestionEtudiants {
private:
    vector<etudiant> etudiants;

public:
    bool etudiantExiste(const etudiant& e) const {
        for (const auto& etu : etudiants) {
            if (etu.compareId(e)) {
               return true;
            }
        }
       return false;
    }

    void ajouterEtudiant(const etudiant &e) {
        for (const auto &etu : etudiants)
            if (etu.compareId(e)) {
                cout << "L'etudiant avec ID " << e.getId() << " existe deja.\n";
                return;
            }
        etudiants.push_back(e);
        cout << "Etudiant ajoute: " << e.getNom() << " (ID: " << e.getId() << ")\n";
    }


 void modifierEtudiant(int id) {
    for (auto& etu : etudiants) {
        if (etu.getId() == id) {
            int choix;
            do {
                cout << "\n--- Modifier etudiant ID " << id << " ---\n";
                cout << "1. Nom\n";
                cout << "2. Prenom\n";
                cout << "3. Filiere\n";
                cout << "4. Niveau\n";
                cout << "0. Terminer\n";
                cout << "Choix: ";
                cin >> choix;
                cin.ignore();

                switch (choix) {
                case 1: {
                    string nom;
                    cout << "Nouveau nom: ";
                    getline(cin, nom);
                    etu.setNom(nom);
                    break;
                }
                case 2: {
                    string prenom;
                    cout << "Nouveau prenom: ";
                    getline(cin, prenom);
                    etu.setPrenom(prenom);
                    break;
                }
                case 3: {
                    string filiere;
                    cout << "Nouvelle filiere: ";
                    getline(cin, filiere);
                    etu.setFiliere(filiere);
                    break;
                }
                case 4: {
                    string niveau;
                    cout << "Nouveau niveau: ";
                    getline(cin, niveau);
                    etu.setNiveau(niveau);
                    break;
                }
                case 0:
                    cout << "Modification terminee.\n";
                    break;
                default:
                    cout << "Choix invalide.\n";
                }
            } while (choix != 0);

            return;
        }
    }

    cout << "Aucun etudiant trouve avec l'ID " << id << ".\n";
 }


       etudiant* rechercherParId(int id) {
        for (auto& etu : etudiants) {
            if (etu.getId() == id) return &etu;
        }
        return nullptr;
    }

    vector<etudiant*> rechercherParNom(const string& nom) {
        vector<etudiant*> result;
        for (auto& etu : etudiants) {
            if (etu.getNom() == nom) result.push_back(&etu);
        }
        return result;
    }

    void afficherEtudiants() const {
        cout << "\nListe des etudiants:\n";
        for (const auto &e : etudiants)
            e.afficher();
    }
    vector<etudiant>& getEtudiants() { 
        return etudiants; 
    }
    void supprimerEtudiant(int id) {
        auto it = remove_if(etudiants.begin(), etudiants.end(),
             [id](const etudiant& e) {
            return e.getId() == id;
        });

        if (it == etudiants.end()) {
            cout << "Etudiant introuvable.\n";
            return;
        }

     etudiants.erase(it, etudiants.end());
        cout << "Etudiant supprime avec succes.\n";
    }

};
class Admnistratif : public personne {
public:
    Admnistratif(string n = "", string p = "", int i = 0)
        : personne(n, p, i) {}


    void ajouterEtudiant(vector<etudiant>& etudiants, const etudiant& e) {
        for (auto& etu : etudiants) {
            if (etu.compareId(e)) {
                cout << "L etudiant avec ID " << e.getId() << " existe deja.\n";
                return;
            }
        }
        etudiants.push_back(e);
        cout << "Etudiant ajoute : " << e.getNom() << " (ID: " << e.getId() << ")\n";
    }

    // Modifier un étudiant
    void modifierEtudiant(vector<etudiant>& etudiants, int id) {
        for (auto& etu : etudiants) {
            if (etu.getId() == id) {
                cout << "Modifier l'etudiant :\n";
                etu.remplir_info();
                cout << "Etudiant modifie.\n";
                return;
            }
        }
        cout << "Etudiant introuvable.\n";
    }

    // Supprimer un étudiant
    void supprimerEtudiant(vector<etudiant>& etudiants, int id) {
        for (size_t i = 0; i < etudiants.size(); i++) {
            if (etudiants[i].getId() == id) {
                etudiants.erase(etudiants.begin() + i);
                cout << "Etudiant supprime.\n";
                return;
            }
        }
        cout << "Etudiant introuvable.\n";
    }

    // Ajouter un professeur
    void ajouterProfesseur(vector<Professeur>& profs, const Professeur& p) {
        for (auto& pr : profs) {
            if (pr.compareId(p)) {
                cout << "Le professeur avec ID " << p.getId() << " existe deja.\n";
                return;
            }
        }
        profs.push_back(p);
        cout << "Professeur ajoute : " << p.getNom() << " (ID: " << p.getId() << ")\n";
    }

    // Modifier un professeur
    void modifierProfesseur(vector<Professeur>& profs, int id) {
        for (auto& pr : profs) {
            if (pr.getId() == id) {
                cout << "Modifier le professeur :\n";
                pr.remplir_info();
                cout << "Professeur modifie.\n";
                return;
            }
        }
        cout << "Professeur introuvable.\n";
    }

    // Supprimer un professeur
    void supprimerProfesseur(vector<Professeur>& profs, int id) {
        for (size_t i = 0; i < profs.size(); i++) {
            if (profs[i].getId() == id) {
                profs.erase(profs.begin() + i);
                cout << "Professeur supprime.\n";
                return;
            }
        }
        cout << "Professeur introuvable.\n";
    }

    void afficher() const override {
        cout << "Administratif: " << nom << " " << prenom << " (ID: " << id << ")" << endl;
    }
};


class Note {
private:
    int id;
    int idEtudiant;
    double valeur;
    string codeCours;
    string typeExam;
public:
    Note(int d, int idE, double v, string  c, string t) : id(d),idEtudiant(idE),valeur(v),codeCours(c),typeExam(t)  {}
    int getId() const { return id;}
    int getIdEtudiant() const { return idEtudiant; }
    double getValeur() const { return valeur; }
    string getcodeCours() const { return codeCours;}
    string getTypeExam() const { return typeExam; }
    void setTypeExam(const string& t) { typeExam = t; }
    void setCodeCours(const string& c) {
        codeCours = c;
    }

    void setIdEtudiant(int id) {
        if (id <= 0) throw invalid_argument("ID etudiant invalide");
        idEtudiant = id;
    }

    void setValeur(double nouvelNote){
        if (nouvelNote < 0 || nouvelNote > 20) throw NoteInvalide();
        valeur=nouvelNote;
    }
    void afficher() const {
        cout << "Etudiant " << idEtudiant
             << " | " << codeCours
             << " | " << typeExam
             << " | Note: " << valeur << endl;
    }
    json toJson() const {
        return json{
            {"id", id}, 
            {"idEtudiant", idEtudiant},
            {"valeur", valeur}, 
            {"codeCours", codeCours},
            {"typeExam", typeExam}
        };
    }

    static Note fromJson(const json& j) {
        return Note(
            j.at("id").get<int>(),
            j.at("idEtudiant").get<int>(),
            j.at("valeur").get<double>(), 
            j.at("codeCours").get<string>(),
            j.value("typeExam", "Examen") 
        );
    }
};
class GestionNotes {
private:
    vector<Note> notes;

public:
    vector<Note> getAllNotes() const { return notes; }

    // Ajouter une note
    void ajouterNote(const Note& n) {
        notes.push_back(n);
        cout << "Note ajoutee a l'etudiant ID " << n.getIdEtudiant() << endl;
    }
     vector<Note> getNotesEtudiant(int idEtudiant) const {
        vector<Note> res;
        for (const auto& n : notes)
            if (n.getIdEtudiant() == idEtudiant)
                res.push_back(n);
        return res;
    }
    // Modifier une note par ID
    void modifierNoteMenu(int idNote) {
        for (auto& n : notes) {
        if (n.getId() == idNote) {

            int choix;
            do {
                cout << "\n=== Modifier Note ID " << idNote << " ===\n";
                cout << "1. Modifier valeur\n";
                cout << "2. Modifier code matiere\n";
                cout << "3. Modifier etudiant\n";
                cout << "4. Modifier type examen\n";
                cout << "0. Retour\n";
                cout << "Choix: ";
                cin >> choix;
                cin.ignore();

                switch (choix) {
                case 1: {
                    double v;
                    cout << "Nouvelle valeur: ";
                    cin >> v; cin.ignore();
                    n.setValeur(v);
                    break;
                }
                case 2: {
                    string c;
                    cout << "Nouveau code matiere: ";
                    getline(cin, c);
                    n.setCodeCours(c);
                    break;
                }
                case 3: {
                    int idE;
                    cout << "Nouvel ID etudiant: ";
                    cin >> idE; cin.ignore();
                    n.setIdEtudiant(idE);
                    break;
                }
                case 4: {
                    string t;
                    cout << "Nouveau type examen: ";
                    getline(cin, t);
                    n.setTypeExam(t);
                    break;
                }
                }
            } while (choix != 0);

            cout << "Note modifiee avec succes.\n";
            return;
        }
    }

    cout << "Note introuvable.\n";
    }



    // Rechercher une note par ID
    Note* rechercherNote(int id) {
        for (auto& n : notes) {
            if (n.getId() == id) return &n;
        }
        return nullptr;
    }
    double moyenneGenerale() const {
    if (notes.empty()) return 0.0;
    
    double somme = 0;
    for (const auto& n : notes) {
        somme += n.getValeur();
    }
    return somme / notes.size();
   }

    // Afficher toutes les notes
    void afficherNotes() const {
        if (notes.empty()) {
            cout << "Aucune note disponible.\n";
            return;
        }
        for (const auto& n : notes) {
            n.afficher();
        }
    }
    void afficherNotesEtudiant(int idEtudiant) const {
        auto liste = getNotesEtudiant(idEtudiant);
        if (liste.empty()) {
            cout << "Aucune note pour cet etudiant.\n";
            return;
        }
        for (const auto& n : liste)
            n.afficher();
    }
    void supprimerNote(int id) {
        auto it = remove_if(notes.begin(), notes.end(),
        [id](const Note& n) {
            return n.getId() == id;
        });

        if (it == notes.end()) {
            cout << "Note introuvable.\n";
            return;
        }

        notes.erase(it, notes.end());
        cout << "Note supprimee avec succes.\n";
    }

};
class Matiere {
private:
    string nom;
    string code;
    int coefficient;
    int idProfesseur;
public:
        Matiere(string n = "", string c = "", int coeff = 1, int idProf = -1)
        : nom(n), code(c), coefficient(coeff), idProfesseur(idProf) {}

    string getNom() const { return nom; }
    string getCode() const { return code; }
    int getCoefficient() const { return coefficient; }
    int getIdProfesseur() const { return idProfesseur; }
    void setIdProfesseur(int id) { idProfesseur = id; }
    void setNom(const string& n) { nom = n; }
    void setCode(const string& c) { code = c; }
    void setCoefficient(int coeff) { coefficient = coeff; }

    void afficher() const {
        cout << "Matiere: " << nom << " (Code: " << code << ", Coeff: " << coefficient;
        if (idProfesseur != -1)
            cout << "  Professeur ID: " << idProfesseur << endl;
        else
            cout << "  Aucun professeur affecte.\n";
        cout << ")" << endl;    
    }

    bool compareCode(const Matiere& m) const {
        return code == m.code;
    }
    json toJson() const {
    return json{
        {"nom", nom},
        {"code", code},
        {"coefficient", coefficient},
        {"idProfesseur", idProfesseur}
    }; 
   }

   static Matiere fromJson(const json& j) {
    return Matiere(
        j.at("nom").get<string>(),
        j.at("code").get<string>(),
        j.at("coefficient").get<int>(),
        j.value("idProfesseur", -1) // SAFE
    );
  }

};
class GestionMatiere {
 private:
    vector<Matiere> matieres;

 public:
    vector<Matiere>& getMatieres() { return matieres; }
    void ajouterMatiere(const Matiere& m) {
        // Vérifie si le code existe déjà
        for (auto& mat : matieres) {
            if (mat.compareCode(m)) {
                throw MatiereExistant();
                return;
            }
        }
        matieres.push_back(m);
        cout << "Matiere ajoutee: " << m.getNom() << endl;
    }
    void affecterProfesseur(const string& codeMatiere, int idProf) {
    for (auto& m : matieres) {
        if (m.getCode() == codeMatiere) {
            m.setIdProfesseur(idProf);
            cout << "Professeur (ID " << idProf 
                 << ") affecte a la matiere " << codeMatiere << endl;
            return;
        }
    }
    cout << "Matiere introuvable.\n";
  }

 void modifierMatiereMenu(const string& code) {
    for (auto& mat : matieres) {
        if (mat.getCode() == code) {
            int choix;
            do {
                cout << "\n--- Modifier Matiere Code " << code << " ---\n";
                cout << "1. Nom\n";
                cout << "2. Coefficient\n";
                cout << "0. Terminer\n";
                cout << "Choix: ";
                cin >> choix; cin.ignore();

                switch(choix) {
                    case 1: {
                        string nouveauNom;
                        cout << "Nouveau nom: ";
                        getline(cin, nouveauNom);
                        mat.setNom(nouveauNom);
                        break;
                    }
                    case 2: {
                        int nouveauCoeff;
                        cout << "Nouveau coefficient: ";
                        cin >> nouveauCoeff; cin.ignore();
                        mat.setCoefficient(nouveauCoeff);
                        break;
                    }
                    case 0:
                        cout << "Modification terminee.\n";
                        break;
                    default:
                        cout << "Choix invalide.\n";
                }
            } while (choix != 0);
            return;
        }
    }
    cout << "Matiere avec code " << code << " introuvable.\n";
 }


    Matiere* rechercherMatiere(const string& code) {
        for (auto& mat : matieres) {
            if (mat.getCode() == code) return &mat;
        }
        return nullptr;
    }

    void supprimerMatiere(const string& code) {
      auto it = remove_if(matieres.begin(), matieres.end(),
          [&](const Matiere& m) {
            return m.getCode() == code;
        });

        if (it == matieres.end()) {
         cout << "Matiere introuvable.\n";
            return;
        }

        matieres.erase(it, matieres.end());
        cout << "Matiere supprimee.\n";
    }


    void afficherToutes() const {
        cout << "Liste des matieres:\n";
        for (const auto& mat : matieres) mat.afficher();
    }
};

class ReleveDeNotes {
private:
    int idEtudiant;
    std::string nomEtudiant;
    std::vector<std::shared_ptr<Note>> notes;

public:
    ReleveDeNotes(int id, const std::string& nom)
        : idEtudiant(id), nomEtudiant(nom) {}

    int getIdEtudiant() const { return idEtudiant; }
    std::string getNomEtudiant() const { return nomEtudiant; }
    void setNomEtudiant(const std::string& nom) { nomEtudiant = nom; }

    // Ajouter une note directement
    void ajouterNote(const std::shared_ptr<Note>& n) {
        // Évite les doublons pour le même cours et type d'examen
        auto it = std::find_if(notes.begin(), notes.end(),
            [&](const std::shared_ptr<Note>& note) {
                return note->getcodeCours() == n->getcodeCours() &&
                       note->getTypeExam() == n->getTypeExam();
            });
        if (it != notes.end()) {
            std::cout << "Note deja existante pour ce cours et type.\n";
            return;
        }
        notes.push_back(n);
    }

    // Ajouter une note via cours + valeur (id auto)
    void ajouterNote(const std::string& matiere, double valeur) {
        static int nextId = 1;
        ajouterNote(std::make_shared<Note>(nextId++, idEtudiant, valeur, matiere, "Examen"));
    }

    void modifierNote(const std::string& matiere, double valeur) {
        for (auto& n : notes) {
            if (n->getcodeCours() == matiere) {
                n->setValeur(valeur);
                return;
            }
        }
        std::cout << "Note pour la matiere " << matiere << " introuvable.\n";
    }

    void supprimerNote(const std::string& matiere) {
        auto it = std::remove_if(notes.begin(), notes.end(),
            [&](const std::shared_ptr<Note>& n) { return n->getcodeCours() == matiere; });
        if (it == notes.end()) {
            std::cout << "Note pour la matiere " << matiere << " introuvable.\n";
            return;
        }
        notes.erase(it, notes.end());
    }

    double moyenneGenerale() const {
        if (notes.empty()) return 0.0;
        double somme = 0.0;
        for (const auto& n : notes) somme += n->getValeur();
        return somme / notes.size();
    }

    void afficher() const {
        std::cout << "\n===== Releve de Notes =====\n";
        std::cout << "Etudiant ID: " << idEtudiant << "\nNom: " << nomEtudiant << "\n";
        if (notes.empty()) {
            std::cout << "Aucune note.\n";
            return;
        }
        for (const auto& n : notes) n->afficher();
        std::cout << "---------------------------\n";
        std::cout << "Moyenne generale: " << moyenneGenerale() << "\n";
    }

    // JSON serialization
    json toJson() const {
        json j;
        j["idEtudiant"] = idEtudiant;
        j["nomEtudiant"] = nomEtudiant;
        j["notes"] = json::array();
        for (const auto& n : notes) j["notes"].push_back(n->toJson());
        return j;
    }

    // JSON deserialization
    static std::shared_ptr<ReleveDeNotes> fromJson(const json& j) {
        auto r = std::make_shared<ReleveDeNotes>(
            j.at("idEtudiant").get<int>(),
            j.at("nomEtudiant").get<std::string>()
        );
        for (const auto& n : j.at("notes"))
            r->ajouterNote(std::make_shared<Note>(Note::fromJson(n)));
        return r;
    }

    std::vector<std::shared_ptr<Note>>& getNotes() { return notes; }
};


class GestionReleveDeNotes {
private:
    vector<shared_ptr<ReleveDeNotes>> releves;

public:
    void ajouterReleve(shared_ptr<ReleveDeNotes> r) {
        for (const auto& rel : releves) {
            if (rel->getIdEtudiant() == r->getIdEtudiant()) {
                throw ReleveExistant();
            }
        }
        releves.push_back(r);
        cout << "Releve ajoute pour " << r->getNomEtudiant() << endl;
    }
    shared_ptr<ReleveDeNotes> rechercherReleve(int id) { 
        for (auto& r : releves) { if (r->getIdEtudiant() == id) 
            return r; 
        } 
        return nullptr; 
    }
    void modifierReleve(int idEtudiant) {
    shared_ptr<ReleveDeNotes> releve = nullptr;

    for (auto& r : releves) {
        if (r->getIdEtudiant() == idEtudiant) {
            releve = r;
            break;
        }
    }

    if (!releve) {
        throw ReleveIntrouvable();
    }

    int choix;
    do {
        cout << "\n--- Modifier Releve de Notes ---\n";
        cout << "1. Modifier le nom de l'etudiant\n";
        cout << "2. Ajouter une note\n";
        cout << "3. Modifier une note\n";
        cout << "4. Supprimer une note\n";
        cout << "0. Quitter\n";
        cout << "Votre choix : ";
        cin >> choix;

        switch (choix) {
            case 1: {
                string nouveauNom;
                cout << "Nouveau nom : ";
                cin.ignore();
                getline(cin, nouveauNom);
                releve->setNomEtudiant(nouveauNom);
                cout << "Nom modifie avec succes.\n";
                break;
            }

            case 2: {
                string matiere;
                double valeur;
                cout << "Matiere : ";
                cin >> matiere;
                cout << "Note : ";
                cin >> valeur;
                releve->ajouterNote(matiere, valeur);
                cout << "Note ajoutee.\n";
                break;
            }

            case 3: {
                string matiere;
                double nouvelleNote;
                cout << "Matiere a modifier : ";
                cin >> matiere;
                cout << "Nouvelle note : ";
                cin >> nouvelleNote;
                releve->modifierNote(matiere, nouvelleNote);
                cout << "Note modifiee.\n";
                break;
            }

            case 4: {
                string matiere;
                cout << "Matiere a supprimer : ";
                cin >> matiere;
                releve->supprimerNote(matiere);
                cout << "Note supprimee.\n";
                break;
            }

            case 0:
                cout << "Modification terminee.\n";
                break;

            default:
                cout << "Choix invalide.\n";
        }

    } while (choix != 0);
}

    void supprimerReleve(int idEtudiant) {
        auto it = remove_if(
            releves.begin(),
            releves.end(),
            [idEtudiant](const shared_ptr<ReleveDeNotes>& r) {
                return r->getIdEtudiant() == idEtudiant;
            }
        );

        if (it == releves.end())
            throw ReleveIntrouvable();

        releves.erase(it, releves.end());
        cout << "Releve supprime avec succes.\n";
    }

    void afficherTous() const {
        for (const auto& r : releves)
            r->afficher();
    }

    vector<shared_ptr<ReleveDeNotes>>& getReleves() {
        return releves;
    }
};

class Groupe {
private:
    int idGroupe;
    string nomGroupe;
    string filiere; 
    vector<shared_ptr<etudiant>> etudiants;

public:
    Groupe(int id, const string& nom, const string& f = "")
        : idGroupe(id), nomGroupe(nom), filiere(f) {}

    int getIdGroupe() const { return idGroupe; }
    string getNomGroupe() const { return nomGroupe; }
    string getFiliere() const { return filiere; } 

    void setFiliere(const string& f) { filiere = f; } 

    vector<shared_ptr<etudiant>>& getEtudiants() { return etudiants; }


    // Setters
    void setNomGroupe(const string& nouveauNom) { nomGroupe = nouveauNom; }
    void setFiliereGroupe(const string& f) { filiere = f; }

    // Student management
    void ajouterEtudiant(shared_ptr<etudiant> e) {
        if (!filiere.empty() && e->getFiliere() != filiere) {
            cout << "Erreur: L'etudiant n'appartient pas a la filiere de ce groupe.\n";
            return;
        }
        for (auto& etu : etudiants) {
            if (etu->getId() == e->getId()) throw GroupeExistant();
        }
        etudiants.push_back(e);
        cout << "Etudiant de l id " << e->getId() << " est ajoute au groupe " << nomGroupe << " de la filiere " << filiere << endl;
    }

    void supprimerEtudiant(int idEtudiant) {
        auto it = remove_if(etudiants.begin(), etudiants.end(),
                            [idEtudiant](const shared_ptr<etudiant>& e) {
                                return e->getId() == idEtudiant;
                            });
        if (it != etudiants.end()) {
            etudiants.erase(it, etudiants.end());
            cout << "Etudiant supprime.\n";
        } else {
            cout << "Etudiant introuvable dans ce groupe.\n";
        }
    }

    // Optional: modify a student in the group
    void modifierEtudiant(int idEtudiant, const string& nouveauNom, const string& nouveauPrenom) {
        for (auto& e : etudiants) {
            if (e->getId() == idEtudiant) {
                e->setNom(nouveauNom);
                e->setPrenom(nouveauPrenom);
                cout << "Etudiant modifie.\n";
                return;
            }
        }
        cout << "Etudiant introuvable.\n";
    }
    void modifierGroupe() {
    int choix;
    do {
        cout << "\n--- Modifier Groupe: " << nomGroupe << " ---\n";
        cout << "1. Changer le nom du groupe\n";
        cout << "2. Changer la filiere du groupe\n";
        cout << "3. Ajouter un etudiant\n";
        cout << "4. Supprimer un etudiant\n";
        cout << "5. Modifier un etudiant\n";
        cout << "0. Retour\n";
        cout << "Votre choix: ";
        cin >> choix;
        cin.ignore();

        switch (choix) {
            case 1: {
                string nouveauNom;
                cout << "Nouveau nom du groupe: ";
                getline(cin, nouveauNom);
                setNomGroupe(nouveauNom);
                cout << "Nom modifie avec succes.\n";
                break;
            }
            case 2: {
                string nouvelleFiliere;
                cout << "Nouvelle filiere: ";
                getline(cin, nouvelleFiliere);
                setFiliere(nouvelleFiliere);
                cout << "Filiere modifiee avec succes.\n";
                break;
            }
            case 3: {
                string nom, prenom, filiere;
                int id;
                cout << "Nom de l'etudiant: ";
                getline(cin, nom);
                cout << "Prenom: ";
                getline(cin, prenom);
                cout << "ID: ";
                cin >> id;
                cin.ignore();
                cout << "Filiere: ";
                getline(cin, filiere);

                auto e = make_shared<etudiant>(nom, prenom, id, filiere, ""); // niveau optionnel
                ajouterEtudiant(e);
                break;
            }
            case 4: {
                int idEtudiant;
                cout << "ID de l'etudiant a supprimer: ";
                cin >> idEtudiant;
                cin.ignore();
                supprimerEtudiant(idEtudiant);
                break;
            }
            case 5: {
                int idEtudiant;
                string nom, prenom;
                cout << "ID de l'etudiant a modifier: ";
                cin >> idEtudiant;
                cin.ignore();
                cout << "Nouveau nom: ";
                getline(cin, nom);
                cout << "Nouveau prenom: ";
                getline(cin, prenom);
                modifierEtudiant(idEtudiant, nom, prenom);
                break;
            }
            case 0:
                cout << "Retour...\n";
                break;
            default:
                cout << "Choix invalide.\n";
        }
    } while (choix != 0);
}

    void afficherEtudiants() const {
        cout << "Groupe " << nomGroupe << " (Filiere: " << filiere << ", ID: " << idGroupe << "):\n";
        for (auto& e : etudiants) {
            cout << "- " << e->getNom() << " " << e->getPrenom() << " (ID: " << e->getId() << ")\n";
        }
    }

    bool contientEtudiant(int idEtudiant) const {
        for (auto& e : etudiants) if (e->getId() == idEtudiant) return true;
        return false;
    }
    json toJson() const {
        json j;
        j["idGroupe"] = idGroupe;
        j["nomGroupe"] = nomGroupe;
        j["filiere"] = filiere;

        j["etudiants"] = json::array();
        for (auto& e : etudiants) {
            j["etudiants"].push_back(e->toJson()); // each etudiant must have toJson()
        }

        return j;
    }


    static shared_ptr<Groupe> fromJson(const json& j) {
        auto g = make_shared<Groupe>(j.at("idGroupe").get<int>(), j.at("nomGroupe").get<string>());
        for(auto& e : j.at("etudiants"))
            g->ajouterEtudiant(make_shared<etudiant>(etudiant::fromJson(e)));
        return g;
    }
};
class GestionGroupe {
private:
    vector<shared_ptr<Groupe>> groupes;

public:
   vector<shared_ptr<Groupe>>& getGroupes() { return groupes; }
    void ajouterGroupe(shared_ptr<Groupe> g) {
        for (auto& grp : groupes) {
            if (grp->getIdGroupe() == g->getIdGroupe()) {
                cout << "Le groupe existe deja.\n";
                return;
            }
        }
        groupes.push_back(g);
        cout << "Groupe ajoute: " << g->getNomGroupe() << endl;
    }

    shared_ptr<Groupe> rechercherGroupe(int id) {
        for (auto& g : groupes) if (g->getIdGroupe() == id) return g;
        return nullptr;
    }
    vector<shared_ptr<Groupe>> getGroupesParFiliere(const string& filiere) {
        vector<shared_ptr<Groupe>> result;
        for (auto& g : groupes) {
            if (g->getFiliere() == filiere)
                result.push_back(g);
        }
        return result;
    }
    shared_ptr<Groupe> rechercherParNomEtFiliere(
        const string& nom,
        const string& filiere
    ) {
        for (auto& g : groupes) {
            if (g->getNomGroupe() == nom &&
                g->getFiliere() == filiere) {
                return g;
            }
        }
        return nullptr;
    }


   void modifierUnGroupe() {
        int id;
        cout << "Entrez l'ID du groupe a modifier: ";
        cin >> id;
        cin.ignore();

        auto g = rechercherGroupe(id);
        if (!g) {
            cout << "Groupe introuvable.\n";
            return;
        }

        g->modifierGroupe(); // call the Groupe method
    }

    void supprimerGroupe(int id) {
        auto it = remove_if(groupes.begin(), groupes.end(),
        [id](const shared_ptr<Groupe>& g) {
            return g->getIdGroupe() == id;
        });

        if (it == groupes.end()) {
            cout << "Groupe introuvable.\n";
            return;
        }

        groupes.erase(it, groupes.end());
        cout << "Groupe supprime.\n";
    }


    void afficherTous() const {
        for (auto& g : groupes) g->afficherEtudiants();
    }
};
class Etablissement {
private:
    string nom;

    shared_ptr<GestionEtudiants> gestionEtudiants;
    shared_ptr<GestionProf> gestionProfesseurs;
    shared_ptr<GestionGroupe> gestionGroupes;
    shared_ptr<GestionMatiere> gestionMatieres;
    shared_ptr<GestionNotes> gestionNotes;
    shared_ptr<GestionReleveDeNotes> gestionReleves;

public:
    // Constructeur
    Etablissement(const string& n = "")
        : nom(n),
          gestionEtudiants(make_shared<GestionEtudiants>()),
          gestionProfesseurs(make_shared<GestionProf>()),
          gestionGroupes(make_shared<GestionGroupe>()),
          gestionMatieres(make_shared<GestionMatiere>()),
          gestionNotes(make_shared<GestionNotes>()),
          gestionReleves(make_shared<GestionReleveDeNotes>()) {}

    // Accesseurs pour les gestionnaires
    shared_ptr<GestionEtudiants> getGestionEtudiants() { return gestionEtudiants; }
    shared_ptr<GestionProf> getGestionProfesseurs() { return gestionProfesseurs; }
    shared_ptr<GestionGroupe> getGestionGroupes() { return gestionGroupes; }
    shared_ptr<GestionMatiere> getGestionMatieres() { return gestionMatieres; }
    shared_ptr<GestionNotes> getGestionNotes() { return gestionNotes;}
    shared_ptr<GestionReleveDeNotes> getGestionReleves() { return gestionReleves; }

    string getNom() const { return nom; }
    void setNom(const string& n) { nom = n; }

    void afficherInfo() const {
        cout << "Etablissement: " << nom << endl;
        cout << "Gestionnaires prets a l'emploi.\n";
    }

};
void menuEtudiants(shared_ptr<GestionEtudiants> gestion,shared_ptr<GestionGroupe> gestionGroupes) {
    int choix;
    do {
        cout << "\n=== Menu Etudiants ===\n";
        cout << "1. Ajouter un etudiant\n";
        cout << "2. Modifier un etudiant\n";
        cout << "3. Rechercher un etudiant par ID\n";
        cout << "4. Rechercher un etudiant par nom\n";
        cout << "5. Afficher tous les etudiants\n";
        cout << "6. Supprimer un etudiant\n";
        cout << "0. Retour\n";
        cout << "Votre choix: ";
        cin >> choix;
        cin.ignore();

        switch (choix) {
            case 1: {
                string nom, prenom, filiere, niveau;
                int id;

                cout << "Nom: ";
                getline(cin, nom);

                cout << "Prenom: ";
                getline(cin, prenom);

                cout << "ID: ";
                cin >> id;
                cin.ignore();

                cout << "Filiere: ";
                getline(cin, filiere);

                cout << "Niveau: ";
                getline(cin, niveau);

                //  Choose or create group (max 30 students)
                shared_ptr<Groupe> groupeChoisi = nullptr;
                string nomGroupe;

                do {
                    cout << "Nom du groupe: ";
                    getline(cin, nomGroupe);

                    // Try to find existing group
                    groupeChoisi = gestionGroupes->rechercherParNomEtFiliere(nomGroupe, filiere);

                    if (groupeChoisi) {
                        // Check if group is full
                        if (groupeChoisi->getEtudiants().size() >= 30) {
                            cout << "Le groupe " << nomGroupe << " est plein (30 étudiants max). "
                                << "Vous devez créer un nouveau groupe.\n";
                            groupeChoisi = nullptr; // reset to force new group
                        }
                    } else {
                        // Group doesn't exist → create it
                        int idGroupe;
                        cout << "ID du nouveau groupe: ";
                        cin >> idGroupe;
                        cin.ignore();

                        groupeChoisi = make_shared<Groupe>(idGroupe, nomGroupe, filiere);
                        gestionGroupes->ajouterGroupe(groupeChoisi);
                        cout << "Groupe " << nomGroupe << " cree.\n";
                    }

                } while (!groupeChoisi); // loop until a valid group is selected/created

                // Create student
                    // Step 1: create a shared pointer once
                    auto studentPtr = make_shared<etudiant>(nom, prenom, id, filiere, niveau);

                    // Step 2: add the same pointer to the group
                    groupeChoisi->ajouterEtudiant(studentPtr);

                    // Step 3: add the student to your global gestion (if needed)
                    gestion->ajouterEtudiant(*studentPtr);

                cout << "Etudiant de l id " << id << " ajoute et affecte au groupe " << nomGroupe << endl;
                json j;
            for (auto& g : gestionGroupes->getGroupes()) {
                j["groupes"].push_back(g->toJson());
            }
            ofstream file("groupes.json");
            file << j.dump(4);
            file.close();
                break;
            }

            case 2: {
                int id;
                cout << "ID de l'etudiant a modifier: "; cin >> id; cin.ignore();
                gestion->modifierEtudiant(id);
                json j;
                for (auto& g : gestionGroupes->getGroupes()) {
                    j["groupes"].push_back(g->toJson());
                }
                ofstream file("groupes.json");
                file << j.dump(4);
                file.close();

            break;
                break;
            }
            case 3: {
                int id;
                cout << "ID de l'etudiant a rechercher: "; cin >> id; cin.ignore();
                etudiant* e = gestion->rechercherParId(id);
                if (e) e->afficher();
                else cout << "Etudiant introuvable.\n";
                break;
            }
            case 4: {
                string nom;
                cout << "Nom de l'etudiant a rechercher: "; getline(cin, nom);
                auto result = gestion->rechercherParNom(nom);
                for (auto& e : result) e->afficher();
                if (result.empty()) cout << "Aucun etudiant trouve.\n";
                break;
            }
            case 5:{
                gestion->afficherEtudiants();
                break;
            }
            case 6: {
                int id;
                cout << "ID de l'etudiant a supprimer: ";
                cin >> id; cin.ignore();
                gestion->supprimerEtudiant(id);
                break;
            }
        
        }
    } while (choix != 0);
}
void menuProfesseurs(shared_ptr<GestionProf> gestion) {
    int choix;
    do {
        cout << "\n=== Menu Professeurs ===\n";
        cout << "1. Ajouter un professeur\n";
        cout << "2. Modifier un professeur\n";
        cout << "3. Rechercher un professeur par ID\n";
        cout << "4. Rechercher un professeur par nom\n";
        cout << "5. Afficher tous les professeurs\n";
        cout << "6. Supprimer un professeur\n";
        cout << "0. Retour\n";
        cout << "Votre choix: ";
        cin >> choix;
        cin.ignore();

        switch (choix) {
        case 1: {
          string nom, prenom;
         int id;
          cout << "Nom: "; getline(cin, nom);
          cout << "Prenom: "; getline(cin, prenom);
         cout << "ID: "; cin >> id; cin.ignore();

         Professeur p(nom, prenom, id);
         gestion->ajouterProfesseur(p);

         break;
     }

        case 2: {
            int id;
            cout << "ID du professeur a modifier: "; cin >> id; cin.ignore();
            gestion->modifierProfesseur(id);
            break;
        }
        case 3: {
            int id;
            cout << "ID du professeur a rechercher: "; cin >> id; cin.ignore();
            Professeur* p = gestion->rechercherParId(id);
            if (p) p->afficher();
            else cout << "Professeur introuvable.\n";
            break;
        }
        case 4: {
            string nom;
            cout << "Nom du professeur a rechercher: "; getline(cin, nom);
            auto result = gestion->rechercherParNom(nom);
            for (auto& p : result) p->afficher();
            if (result.empty()) cout << "Aucun professeur trouve.\n";
            break;
        }
        case 5:{
            gestion->afficherTous();
            break;
        }
        case 6: {
            int id;
            cout << "ID du professeur a supprimer: ";
            cin >> id; cin.ignore();
            gestion->supprimerProfesseur(id);
            break;
        }

        }
    } while (choix != 0);
}
void menuNotes(shared_ptr<GestionNotes> gestion) {
    int choix;
    do {
        cout << "\n=== Menu Notes ===\n";
        cout << "1. Ajouter une note\n";
        cout << "2. Modifier une note\n";
        cout << "3. Supprimer une note\n";
        cout << "4. Afficher toutes les notes\n";
        cout << "0. Retour\n";
        cout << "Votre choix: ";
        cin >> choix; 
        cin.ignore();

        switch (choix) {

        case 1: {
            int idNote, idEtudiant;
            double valeur;
            string code, type;

            cout << "ID de la note: ";
            cin >> idNote;

            cout << "ID de l'etudiant: ";
            cin >> idEtudiant;

            cout << "Valeur: ";
            cin >> valeur;
            cin.ignore();

            cout << "Code du cours: ";
            getline(cin, code);

            cout << "Type d'examen (DS / TP / Examen): ";
            getline(cin, type);

            try {
                gestion->ajouterNote(Note(idNote, idEtudiant, valeur, code, type));
            }
            catch (const NoteInvalide& e) {
                cout << e.what() << endl;
            }
            catch (const exception& e) {
                cout << "Erreur: " << e.what() << endl;
            }

            break;
        }

        case 2: {
            int id;
            cout << "ID de la note a modifier: ";
            cin >> id;
            cin.ignore();

            gestion->modifierNoteMenu(id);   
            break;
        }

        case 3: {
            int id;
            cout << "ID de la note a supprimer: ";
            cin >> id;
            cin.ignore();

            gestion->supprimerNote(id);
            break;
        }

        case 4:
            gestion->afficherNotes();
            break;
        }

    } while (choix != 0);
}


void menuGroupes(shared_ptr<GestionGroupe> gestion) {
    int choix;
    do {
        cout << "\n=== Menu Groupes ===\n";
        cout << "1. Ajouter un groupe\n";
        cout << "2. Rechercher un groupe par ID\n";
        cout << "3. Supprimer un groupe\n";
        cout << "4. Afficher tous les groupes\n";
        cout << "5. Modifier un groupe\n"; // NEW
        cout << "0. Retour\n";
        cout << "Votre choix: ";
        cin >> choix;
        cin.ignore();

        switch (choix) {
            case 1: { // Ajouter un groupe
                int id;
                string nom, filiere;
                cout << "ID du groupe: "; cin >> id; cin.ignore();
                cout << "Nom du groupe: "; getline(cin, nom);
                cout << "Filiere du groupe: "; getline(cin, filiere); 

                auto g = make_shared<Groupe>(id, nom, filiere);
                gestion->ajouterGroupe(g);
                break;
            }

            case 2: { // Rechercher un groupe
                int id;
                cout << "ID du groupe: "; cin >> id; cin.ignore();
                auto g = gestion->rechercherGroupe(id);
                if (g) g->afficherEtudiants();
                else cout << "Groupe introuvable.\n";
                break;
            }

            case 3: { // Supprimer un groupe
                int id;
                cout << "ID du groupe a supprimer: "; cin >> id; cin.ignore();
                gestion->supprimerGroupe(id);
                break;
            }

            case 4: { // Afficher tous les groupes
                gestion->afficherTous();
                break;
            }

            case 5: { // Modifier un groupe
                int id;
                cout << "ID du groupe a modifier: "; cin >> id; cin.ignore();
                auto g = gestion->rechercherGroupe(id);
                if (g) g->modifierGroupe(); // call Groupe's modifier method
                else cout << "Groupe introuvable.\n";
                break;
            }

            case 0:
                cout << "Retour...\n";
                break;

            default:
                cout << "Choix invalide.\n";
        }
    } while (choix != 0);
}

// Helper function to calculate a student's average
double calculerMoyenne(const etudiant& e, shared_ptr<GestionNotes> gestionNotes) {
    auto notes = gestionNotes->getNotesEtudiant(e.getId());
    if (notes.empty()) return 0.0;

    double somme = 0.0;
    for (const auto& n : notes) {
        somme += n.getValeur();
    }
    return somme / notes.size();
}

// Function to display students sorted by average and save to JSON
void afficherEtudiantsParMoyenneEtJson(shared_ptr<GestionEtudiants> gestionEtudiants,
                                       shared_ptr<GestionNotes> gestionNotes,
                                       const string& cheminFichier = "etudiants_par_moyenne.json") {
    auto etudiants = gestionEtudiants->getEtudiants();

    // Trier décroissant par moyenne
    sort(etudiants.begin(), etudiants.end(), [&](const etudiant& a, const etudiant& b) {
        return calculerMoyenne(a, gestionNotes) > calculerMoyenne(b, gestionNotes);
    });

    // Affichage console
    cout << "Etudiants tries par moyenne generale (du meilleur au pire):\n";
    for (auto& e : etudiants) {
        e.afficher();
        cout << "Moyenne: " << calculerMoyenne(e, gestionNotes) << "\n\n";
    }

    // Enregistrement JSON
    json jArray = json::array();
    for (auto& e : etudiants) {
        json j = e.toJson();
        j["moyenne"] = calculerMoyenne(e, gestionNotes); // ajouter la moyenne
        jArray.push_back(j);
    }

    ofstream f(cheminFichier);
    if (f.is_open()) {
        f << jArray.dump(4);
        f.close();
        cout << "Etudiants tries sauvegardes dans " << cheminFichier << "\n";
    } else {
        cout << "Impossible de sauvegarder le fichier.\n";
    }
}

int main() {
    
    // Crée l'établissement
    Etablissement etab("Mon Etablissement");

    int choix;
 json j;

 // Load existing data if file exists
 ifstream fin("C:/Users/kanbo/Desktop/cpp_projet/data.json");
   if (fin.is_open()) {
      fin >> j;
      fin.close();

    // Load students
    if (j.contains("etudiants")) {
        for (const auto& je : j["etudiants"])
            etab.getGestionEtudiants()->ajouterEtudiant(etudiant::fromJson(je));
    }

    // Load professors
    if (j.contains("professeurs")) {
        for (const auto& jp : j["professeurs"])
            etab.getGestionProfesseurs()->ajouterProfesseur(Professeur::fromJson(jp));
    }

    // Load matieres
    if (j.contains("matieres")) {
        for (const auto& jm : j["matieres"])
            etab.getGestionMatieres()->getMatieres().push_back(Matiere::fromJson(jm));
    }

    // Load notes
    if (j.contains("notes")) {
        for (const auto& jn : j["notes"])
            etab.getGestionNotes()->ajouterNote(Note::fromJson(jn));
    }

    // Load releves
    if (j.contains("releves")) {
        for (const auto& jr : j["releves"])
            etab.getGestionReleves()->ajouterReleve(ReleveDeNotes::fromJson(jr));
    }

    // Load groupes
    if (j.contains("groupes")) {
        for (const auto& jg : j["groupes"])
            etab.getGestionGroupes()->getGroupes().push_back(Groupe::fromJson(jg));
    }
}

      
  try {
    do {
        cout << "\n=== Menu Principal ===\n";
        cout << "1. Gestion des etudiants\n";
        cout << "2. Gestion des professeurs\n";
        cout << "3. Gestion des groupes\n";
        cout << "4. Gestion des matieres\n";
        cout << "5. Gestion des releves de notes\n";
        cout << "7. Gestion des notes\n";
        cout << "8. Afficher les etudiants trier par moyenne\n";
        cout << "0. Quitter\n";
        cout << "Votre choix: ";
        cin >> choix;
        cin.ignore();

        switch (choix) {
        case 1:
        
            menuEtudiants(etab.getGestionEtudiants(), etab.getGestionGroupes());
            break;
        case 2:
            menuProfesseurs(etab.getGestionProfesseurs());
            break;
        case 3:
            menuGroupes(etab.getGestionGroupes());
            break;
        case 4: {
            int c;
            do {
                cout << "\n=== Menu Matieres ===\n";
                cout << "1. Ajouter matiere\n";
                cout << "2. Modifier matiere\n";
                cout << "3. Supprimer matiere\n";
                cout << "4. Rechercher matiere\n";
                cout << "5. Afficher toutes les matieres\n";
                cout << "6. Affecter professeur a une matiere\n";
                cout << "-1. Retour\n";
                cout << "Votre choix: ";
                cin >> c; cin.ignore();

                switch(c) {

                    case 1: { // Ajouter matière
                        try {
                            string nom, code; int coeff;
                            cout << "Nom: "; getline(cin, nom);
                            cout << "Code: "; getline(cin, code);
                            cout << "Coefficient: "; cin >> coeff; cin.ignore();
                            etab.getGestionMatieres()->ajouterMatiere(Matiere(nom, code, coeff));
                            cout << "Matiere ajoutee avec succes.\n";
                        }
                        catch (const MatiereExistant& e) {
                            cout << e.what() << endl;
                        }
                        catch (const exception& e) {
                            cout << "Erreur: " << e.what() << endl;
                        }
                        break;
                    }

                    case 2: { // Modifier matière
                        try {
                            string code;
                            cout << "Code de la matiere a modifier: ";
                            getline(cin, code);
                            etab.getGestionMatieres()->modifierMatiereMenu(code);
                        }
                        catch (const MatiereIntrouvable& e) {
                            cout << e.what() << endl;
                        }
                        catch (const exception& e) {
                            cout << "Erreur: " << e.what() << endl;
                        }
                        break;
                    }

                    case 3: { // Supprimer matière
                        try {
                            string code;
                            cout << "Code de la matiere a supprimer: "; 
                            getline(cin, code);
                            etab.getGestionMatieres()->supprimerMatiere(code);
                            cout << "Matiere supprimee avec succes.\n";
                        }
                        catch (const MatiereIntrouvable& e) {
                            cout << e.what() << endl;
                        }
                        catch (const exception& e) {
                            cout << "Erreur: " << e.what() << endl;
                        }
                        break;
                    }

                    case 4: { // Rechercher matière
                        string code;
                        cout << "Code de la matiere a rechercher: "; 
                        getline(cin, code);
                        Matiere* m = etab.getGestionMatieres()->rechercherMatiere(code);
                        if(m) m->afficher();
                        else cout << "Matiere introuvable.\n";
                        break;
                    }

                    case 5: // Afficher toutes
                        etab.getGestionMatieres()->afficherToutes();
                        break;

                    case 6: { // Affecter professeur
                        try {
                            string code;
                            int idProf;
                            cout << "Code de la matiere: ";
                            getline(cin, code);
                            cout << "ID du professeur: ";
                            cin >> idProf; cin.ignore();
                            etab.getGestionMatieres()->affecterProfesseur(code, idProf);
                            cout << "Professeur affecte avec succes.\n";
                        }
                        catch (const MatiereIntrouvable& e) {
                            cout << e.what() << endl;
                        }
                        catch (const ProfesseurIntrouvable& e) {
                            cout << e.what() << endl;
                        }
                        catch (const exception& e) {
                            cout << "Erreur: " << e.what() << endl;
                        }
                        break;
                    }

                    case -1:
                        cout << "Retour au menu principal.\n";
                        break;

                    default:
                        cout << "Choix invalide.\n";
                }

            } while(c != -1);
            break;
        }

        case 5: {
            // Menu Relevés de notes
            int c;
            do {
                cout << "\n=== Menu Releves de notes ===\n";
                cout << "1. Ajouter releve\n";
                cout << "2. Ajouter note a un releve\n";
                cout << "3. Modifier un releve\n";  
                cout << "4. Supprimer releve\n";
                cout << "5. Afficher tous les releves\n";
                cout << "0. Retour\n";
                cout << "Votre choix: ";
                cin >> c;
                cin.ignore();


                switch(c) {

                case 1: {   // Ajouter releve
                    int id;
                    string nom;

                    cout << "ID de l'etudiant: ";
                    cin >> id;
                    cin.ignore();

                    cout << "Nom de l'etudiant: ";
                    getline(cin, nom);

                    auto r = make_shared<ReleveDeNotes>(id, nom);
                    etab.getGestionReleves()->ajouterReleve(r);
                    break;
                }

                case 2: {   // Ajouter note
                    int id;
                    cout << "ID de l'etudiant: ";
                    cin >> id;
                    cin.ignore();

                    auto r = etab.getGestionReleves()->rechercherReleve(id);
                    if (!r) {
                        cout << "Releve introuvable.\n";
                        break;
                    }

                    int idNote;
                    string code, type;
                    double note;

                    cout << "ID de la note: ";
                    cin >> idNote;
                    cin.ignore();

                    cout << "Code du cours: ";
                    getline(cin, code);

                    cout << "Type d'examen (DS / TP / Examen): ";
                    getline(cin, type);

                    cout << "Note: ";
                    cin >> note;
                    cin.ignore();

                    r->ajouterNote(make_shared<Note>(idNote, id, note, code, type));
                    break;
                }

                case 3: {   //  Modifier releve
                    int id;
                    cout << "ID de l'etudiant du releve a modifier: ";
                    cin >> id;
                    cin.ignore();

                    etab.getGestionReleves()->modifierReleve(id);
                    break;
                }

                case 4: {   // Supprimer releve
                    int id;
                    cout << "ID de l'etudiant du releve a supprimer: ";
                    cin >> id;
                    cin.ignore();

                    etab.getGestionReleves()->supprimerReleve(id);
                    break;
                }

                case 5: {   // Afficher
                    etab.getGestionReleves()->afficherTous();
                    break;
                }

                case 0:
                    cout << "Retour au menu precedent.\n";
                    break;

                default:
                    cout << "Choix invalide.\n";
                }

            } while(c != 0);
            break;
        }
        case 7:{
            menuNotes(etab.getGestionNotes());
            break;
        }
        case 8: {
            auto gestionEtudiants = etab.getGestionEtudiants();
            auto gestionNotes = etab.getGestionNotes();

            auto etudiants = gestionEtudiants->getEtudiants(); // copie pour trier

            // Trier décroissant par moyenne
            sort(etudiants.begin(), etudiants.end(), [&](const etudiant& a, const etudiant& b) {
                return calculerMoyenne(a, gestionNotes) > calculerMoyenne(b, gestionNotes);
            });

            cout << "\n=== Etudiants tries par moyenne generale ===\n";
            for (const auto& e : etudiants) {
                double moyenne = calculerMoyenne(e, gestionNotes);
                e.afficher();
                cout << "Moyenne: " << moyenne << "\n\n";
            }

            // Enregistrer dans JSON
            json jArray = json::array();
            for (const auto& e : etudiants) {
                json j = e.toJson();
                j["moyenne"] = calculerMoyenne(e, gestionNotes);
                jArray.push_back(j);
            }

            const string cheminFichier = "C:/Users/kanbo/Desktop/cpp_projet/etudiants_par_moyenne.json";
            ofstream f(cheminFichier);
            if (f.is_open()) {
                f << jArray.dump(4); // belle indentation
                f.close();
                cout << "Etudiants tries sauvegardes dans " << cheminFichier << "\n";
            } else {
                cout << "Impossible de sauvegarder le fichier.\n";
            }

            break;
        }

        case 0:
            cout << "Au revoir !\n";
            break;
        }
    } while (choix != 0);
    } catch (const exception &e) {
        cout << "Erreur detectee : " << e.what() << endl;
    }
 
    j = json::object();   // RESET JSON

    // Étudiants
    j["etudiants"] = json::array();
    for (const auto& e : etab.getGestionEtudiants()->getEtudiants())
        j["etudiants"].push_back(e.toJson());

    // Professeurs
    j["professeurs"] = json::array();
    for (const auto& p : etab.getGestionProfesseurs()->getProfs())
        j["professeurs"].push_back(p.toJson());

    // Matières
    j["matieres"] = json::array();
    for (const auto& m : etab.getGestionMatieres()->getMatieres())
        j["matieres"].push_back(m.toJson());

    // Notes
    j["notes"] = json::array();
    for (const auto& n : etab.getGestionNotes()->getAllNotes()) {
        json jn = n.toJson();  // JSON de base de la note
        
        // Ajouter le nom de l'étudiant
        etudiant* e = etab.getGestionEtudiants()->rechercherParId(n.getIdEtudiant());
        if (e) {
            jn["nomEtudiant"] = e->getNom(); // ajouter nom
        } else {
            jn["nomEtudiant"] = "Inconnu";
        }

        j["notes"].push_back(jn);
    }


    // Groupes
    j["groupes"] = json::array();
    for (const auto& g : etab.getGestionGroupes()->getGroupes())
        j["groupes"].push_back(g->toJson());

    // Relevés
    j["releves"] = json::array();
    for (const auto& r : etab.getGestionReleves()->getReleves())
        j["releves"].push_back(r->toJson());

    // Save to file
    ofstream f("C:/Users/kanbo/Desktop/cpp_projet/data.json");
    if(f.is_open()) {
        f << j.dump(4); // dump(4) pour un JSON lisible
        f.close();
    } else {
        cout << "Impossible d'ouvrir le fichier pour la sauvegarde.\n";
    }

    cout << "Donnees sauvegardees dans data.json\n";
    return 0;
}
