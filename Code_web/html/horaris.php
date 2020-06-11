<html>
	<head>
		<link rel="stylesheet" type="text/css" href="css/style.css"/>
		<link rel="shortcut icon" href="img/pill.ico" type="image/x-icon"/>
		<title>Chemist Bot</title>
	</head>

	<body>
		<nav>
			<ul>
				<li><a href="medicament.php">Medicaments</a></li>
				<li><a href="dispensar.php">Dispensar</a></li>
				<li><a href="horaris.php" class="actual">Horaris programats</a></li>
				<li><a href="perfil.php">Perfil</a></li>
			</ul>
		</nav>
		
		<table width=100% cellspacing=0 id="horaris">
		<?php
			include_once 'private/db.php';

			// Mirem si està logejat
			session_start();

			if ($_SESSION['estat'] == 0){
				header("Location:index.php");
			}

			// Variables
			$usuari = $_SESSION['id_usuari'];
			$res_hor = mysqli_query($conn, "SELECT *, (SELECT usuari FROM usuaris WHERE id = id_user) nom_user FROM horaris;");
			$dies = array('dilluns', 'dimarts', 'dimecres', 'dijous', 'divendres', 'dissabte', 'diumenge');
			$color = array('parell', 'imparell');
			$contador = 0;
			
			// Es mira que hi hagui horaris (si no hi ha medicaments, tampoc hi poden haver horaris)
			if (mysqli_num_rows($res_hor) > 0) {

				// Es mostra només l'horari de l'usuari logejat (a menys que sigui admin)
				while($row = mysqli_fetch_array($res_hor)) {
					
					// Guardem els valors d'entrada
					$dia = $dies[$row["dia_setmana"]-1];
					$hora = $row["hora"];
					$quantitat = $row["quantitat"];
					
					// Variables per omplir
					$nom_medicament;
					$id_usuari = $row["id_user"];
					$nom_usuari = $row["nom_user"];
					
					
					// Només imprimim els horaris de l'usuari logejat, a menys que sigui l'admin
					if ($id_usuari == $usuari || $usuari == 1) {
						
						// Contador per alternar els colors
						$contador++; 
						
						// Consulta
						$query_med = mysqli_query($conn, "SELECT * FROM medicaments WHERE id = " . $row["id_med"]);

						// Canviem el color de cada fila
						echo "<tr> <td class='" . $color[$contador%2] . "' width=100%> ";

						// Guardem el nom del medicament i de l'usuari que ha afegit l'orari
						while($row = mysqli_fetch_array($query_med)) {
							$nom_medicament = $row["nom"];
						}

						// Array per passar-li a la funció i cridarla
						$contingut = array($usuari, $nom_usuari, $quantitat, $nom_medicament, $dia, $hora);
						imprimir($contingut);
					}
				}

			} else {
				echo "No hi ha cap dia programat.";
			}
			
			// Funció per imprimir cada fila
			function imprimir($contingut){

				// Si és admin, diu qui ha afegit cada horari
				if ($contingut[0] == 1){
					echo "L'usuari <b>" . $contingut[1] . "</b> té programat dispensar ";
				} else {
					echo "Està programat dispensar ";
				}
				
				echo "<b>" . $contingut[2] . "</b> de <b>" . $contingut[3] . "</b> cada <b>" 
					. $contingut[4] . "</b> a les <b>" . $contingut[5] . "</b>. ";

			}
		?>
		</table>
	</body>
</html>