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
				<li><a href="dispensar.php" class="actual">Dispensar</a></li>
				<li><a href="horaris.php">Horaris programats</a></li>
				<li><a href="perfil.php">Perfil</a></li>
			</ul>
		</nav>
		
		<table width=100% height=92% cellspacing=0>
			<!-- VISTA - AFEGIR HORARI -->
			<tr>
				<td class="imparell" width = 33%>
					<table border = 0px width=100%>
					<tr>
						<td>
							<h2> Afegir nou horari </h2>
						</td>
					</tr>

					<form action="#" method="post"> 
						<tr>
							<td>
								<select name="dia_setmana" id="dia_setmana">
									<option value="1"> Dilluns 	</option>
									<option value="2"> Dimarts 	</option>
									<option value="3"> Dimecres 	</option>
									<option value="4"> Dijous 	</option>
									<option value="5"> Divendres 	</option>
									<option value="6"> Dissabte 	</option>
									<option value="7"> Diumenge 	</option>
								</select>
							</td>
						</tr>
						
						<tr>
							<td>
								<input placeholder="Hora" type="text" name="hora" />
							</td>
						</tr>
						
						<tr>
							<td>
								<input placeholder="Quantitat" type="text" name="quantitat" />
							</td>
						</tr>

						<tr>
							<td> 						
								<select name="medicament" size=5>
								<?php
									include_once 'private/db.php';
									$query = mysqli_query($conn, "SELECT * FROM medicaments");
									$row = $conn->query($query);

									// Si no hi ha cap, imprimim un valor predeterminat
									if (mysqli_num_rows($query) > 0) {
										// Imprimim tots els medicaments que hi ha
										while($row = mysqli_fetch_array($query)) {
											echo "<option value =". $row['id'] ."> " . $row['nom'] . "</option>";	
										}
									} else {
										echo "<option value ='none'>Sense medicaments</option>";
									}
								?>
								</select> 
							</td>
						</tr>

						<tr>
							<td>
								<input type="submit" id="boto" name="afegir" value="Afegir"/>
							</td>
						</tr>
					</form>
				</table>
				</td>
			</tr>
			
			<!-- VISTA - ESBORRAR HORARI -->
			<tr>
				<td class="parell" width = 33%>
					<table border = 0px width=100%>
						<tr>
							<td>
								<h2> Esborrar horari </h2>
							</td>
						</tr>
					
						<form action="#" method="post"> 
							<tr>
								<td>
									<select name="esborrat" size=5>
									<?php
										// Mirem si està logejat
										session_start();

										if ($_SESSION['estat'] == 0){
											header("Location:index.php");
										}

										include_once 'private/db.php';
										
										// Variables
										$query = mysqli_query($conn, "SELECT *, (SELECT nom FROM medicaments WHERE id = id_med) nom FROM horaris
										WHERE id_user = " . $_SESSION['id_usuari'] . " or " . $_SESSION['id_usuari'] . " = 1");
										$row = $conn->query($query);
										$contador = $row->num_rows;
										$dies = array('dilluns', 'dimarts', 'dimecres', 'dijous', 'divendres', 'dissabte', 'diumenge');
										
										// Si no hi ha cap, imprimim un valor predeterminat
										if (mysqli_num_rows($query) > 0) {
											
											// Imprimim tots els horaris programats que hi ha
											while($row = mysqli_fetch_array($query)) {
												echo "<option value = '" . $row['id'] . "'> " . $row['nom'] . " cada " . $dies[$row['dia_setmana']-1]    
													. " a les " .$row['hora'] . "</option>";	
											}
										} else {
											echo "<option value ='none'>Sense horaris programats</option>";
										}
									?>
									</select> 
								</td>
							</tr>
							
							<tr>
								<td> 
									<input type="submit" id="boto" name="esborrar" value="Esborrar"/>
								</td>
							</tr>
	
						</form>
					</table>
				</td>
			</tr>
				
			<!-- VISTA - DISPENSAR MANUALMENT -->
			<tr>
				<td class="imparell" width = 33%>
					<table border = 0px width=100%>
						<tr>
							<td>
								<h2> Dispensar manualment </h2>
							</td>
						</tr>
						
						<form action="#" method="post">
							<tr>
								<td>
									<input placeholder="Cartutx" type="text" name="man_cartutx"/>
								</td>
							</tr>
							
							<tr>
								<td>
									<input placeholder="Quantitat" type="text" name="man_quantitat" />
								</td>
							</tr>
							
							<tr>
								<td>
									<input type="submit" id="boto" name="boto" value="Dosificar"/>
								</td>
							</tr>
						</form>
					</table> 
				</td>
			</tr>

			<!-- CONTROLADORS -->
			<?php
				include_once 'private/db.php';
				session_start();
			
				// Variables
				$id = 0;
				$id_usuari = $_SESSION['id_usuari'];
				$hora = mysqli_real_escape_string($conn, $_POST['hora']);
				$id_med = mysqli_real_escape_string($conn, $_POST['medicament']);
				$quantitat = mysqli_real_escape_string($conn, $_POST['quantitat']);
				$quantitat_disponible = 0;
			
				$man_cartutx = mysqli_real_escape_string($conn, $_POST['man_cartutx']);
				$man_quantitat = mysqli_real_escape_string($conn, $_POST['man_quantitat']);

				// Mirem la ultima id per utilitzar la següent
				$query = mysqli_query($conn, "SELECT max(id) id FROM horaris");
				while($row = mysqli_fetch_array($query)) {
					$id = $row["id"] + 1;
				}		
			
				// CONTROLADOR - AFEGIR NOU HORARI
				if ( isset($_REQUEST['afegir']) ){

					// Mirem quants medicaments hi han a la base de dades					
					$query = mysqli_query($conn, "SELECT *, (SELECT SUM(quantitat) FROM horaris WHERE id_med = m.id) 
											gastat from medicaments m WHERE id = " . $_REQUEST["medicament"]);
					$row = $conn->query($query);
					if (mysqli_num_rows($query) > 0) {
						while($row = mysqli_fetch_array($query)) {
							$quantitat_disponible = $row['quantitat'] - $row['gastat'];	
						}
					}

					// Si no ha elegit medicament o ha escrit 0 en quantitat
					if (is_null($_REQUEST["medicament"]) || $quantitat == 0){
						echo "<script> alert('No has elegit el medicament o la quantitat està buida'); </script>";
						
					// Revisa si, sumant tots els que ja s'utilitzen 1 cop en els horaris, hi ha prous
					} elseif ($quantitat > $quantitat_disponible) {
						echo "<script> alert('No hi ha prous medicaments, queden $quantitat_disponible'); </script>";
					} else {
						echo "<script> alert('medicaments disponibles $quantitat_disponible'); </script>";
						// Ho afegim a la base de dades
						$sql = "INSERT INTO horaris VALUES ($id, $id_med, $id_usuari, " . $_REQUEST['dia_setmana'] . ", '$hora', $quantitat)";

						if (!$conn->query($sql) === TRUE) {
							echo "Error: " . $sql . "<br>" . $conn->error;
						}

						// Actualtizem el crontab
						actualitzat(1, $conn);
						
						header('Location: dispensar.php');	// Refresc	
					}
								

				}

				// CONTROLADOR - ESBORRAR HORARI
				if ( isset($_REQUEST['esborrar']) ){

					// Ho borrem de la BDD
					$sql = "DELETE FROM horaris WHERE id = " . $_REQUEST['esborrat'];
					header('Location: dispensar.php');	// Refresc	

					if (!$conn->query($sql) === TRUE) {
						echo "Error: " . $sql . "<br>" . $conn->error;
					}

					// Actualtizem el crontab
					actualitzat(0, $conn);
				}

				// Actualitza el crontab
				function actualitzat($opcio, $conn){

					// Esborrar
					if ($opcio == 0){
						shell_exec("crontab -r -u www-data");
					}

					// Afegeix tots els horaris que troba a la bd amb el cartutx corresponent	
					$query = mysqli_query($conn, "SELECT *, (SELECT cartutx FROM medicaments  WHERE id = id_med) cartutx FROM horaris ORDER BY 4, 5;");
					$row = $conn->query($query);
					$total = "";
					$horaris = array();
					$quant_horaris = 0;
					$coincideix = false;
						
					if (mysqli_num_rows($query) > 0) {				
						// Tornem a posar tots els horaris que hi ha en el cron
						while($row = mysqli_fetch_array($query)) {
	
							// Split per l'hora i minuts
							$arrHorari = explode(":", $row['hora']);
							$hora = $arrHorari[0];						
							$minuts = $arrHorari[1];
							$cartutx = $row['cartutx'];
							$quantitat = $row['quantitat'];

							// Evitem valors null
							if (is_null($minuts)){ $minuts = 0;	}
							
							// Es guarden tots els horaris en un array
							$horaris[0][$quant_horaris] = $row['dia_setmana'];
							$horaris[1][$quant_horaris] = $hora;
							$horaris[2][$quant_horaris] = $minuts;
							$horaris[3][$quant_horaris] = $cartutx;
							$horaris[4][$quant_horaris] = $quantitat;
							
							$quant_horaris++;
						}	

						// Si varios horaris coincideixen, dispensará un darrera l'altre
						for ($i = 0; $i < $quant_horaris; $i++){
							
							// Si no coincideix, envia el comando normal
							if (!$coincideix){
								$total = $horaris[3][$i] . ' ' . $horaris[4][$i];
							}
							
							// Si coincideix amb el següent, es van unint fins que deixin de coincidir
							if ( ($horaris[0][$i] == $horaris[0][$i+1]) &&
							   	 ($horaris[1][$i] == $horaris[1][$i+1]) &&
								 ($horaris[2][$i] == $horaris[2][$i+1]) ){

								$total .= ' ' . $horaris[3][$i+1] . ' ' . $horaris[4][$i+1];
								$coincideix = true;
								
							} else {

								$coincideix = false;

								// Actualtizem el crontab amb tots els valors
								shell_exec('(crontab -l 2>/dev/null; echo "' . $horaris[2][$i] . ' ' . $horaris[1][$i] . ' * * ' . $horaris[0][$i] 
										   . ' python /var/www/html/connector.py dosificar ' . $total  . ' 2>&1") | crontab -');
														
							}
						}
					}
				}
								
				// CONTROLADOR - DISPENSAR MANUALMENT			
				$query = mysqli_query($conn, "SELECT * FROM medicaments WHERE cartutx = " . $cartutx);
				$row = $conn->query($query);
				$quantitat_disponible = 0;
			
				// Mirem quants medicaments hi han a la base de dades
				if (mysqli_num_rows($query) > 0) {
					while($row = mysqli_fetch_array($query)) {
						$quantitat_disponible = $row['quantitat'];	
					}
				}

				// Si apreta el botó
				if ( isset($_REQUEST['boto']) ){
					if ($quantitat_disponible < $quantitat){
						echo "<script> alert('No hi ha prous medicaments'); </script>";
					} else {
					   boto($man_cartutx, $man_quantitat);
					}
				}

				function boto($man_cartutx, $man_quantitat) { 
					shell_exec("python /var/www/html/connector.py dosificar " . $man_cartutx . " " . $man_quantitat ." 2>&1");
					
				}
			?>
		</table>
	</body>
</html>