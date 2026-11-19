python -m venv .venv
.\.venv\Scripts\Activate.ps1

<!-- Installer django -->
<!-- Installer djangorestframework pour faire le REST API  -->

python -m pip install django djangorestframework

<!-- Creer Projet -->

django-admin startproject projet .

<!-- Creer App associee au projet -->

django-admin startapp DHT

<!-- Lancer l'app sur notre serveur local  -->

python manage.py runserver 10.9.11.200:8000

<!-- Ajouter files urls.py et api.py dans DHT App -->

<!-- Configurer settings dans projet pour qu'il connait les deux DHT ET RESTFramework -->

INSTALLED_APPS = [

    'DHT',
    'rest_framework',

]

<!-- Definir path de notre App dans projet -->

urlpatterns = [
path('',include('DHT.urls')),
]

<!-- Creer View + Url pour creer une api dans DHT + Model class DHt11 avec ces champs  -->

<!-- Creer une BD PostgreSql Migrer le model Dht11 -->

python manage.py makemigrations
python manage.py migrate

<!-- Ajouter le model au niveu admin.py -->

admin.site.register(models.Dht11)

<!-- Creer superuser -->

python manage.py createsuperuser admin admin

<!-- Arduino  -->

4 pers : Admin : Configurer seuil surveillance et creer users donner acces a notre app chacun son role chaque role par ordre
incident detecteur fct + compteur (count nbr d'incidents if(incident) count++ else 0 car temp est devenu normal)
Si incident anormal : donc alerte vers op1 si depaasse un seuil de compteur = 4 op2 puis compteur = 7 op3 admin
Journal dincident : Incident : Sa duree (Rapport [debut : fin : duree d'Incident] est ce que les operateurs ont intervenis ou non donc il valide l'intervention + possibilite d'ajouter un commentaire entre op)
Python anywhere pour deploiement backend
