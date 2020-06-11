    <?php  
        require('db.php');
        
        if (!isset($_POST['login']) ){

            // Es guarden els valors
            $username = mysqli_real_escape_string($conn, $_POST['usuari']);
            $password =  mysqli_real_escape_string($conn, $_POST['contrasenya']);
           	
            // Es crea la consulta SQL
            $query = "SELECT * FROM usuaris WHERE usuari='$username' and password='$password'";

            // S'executa la consulta en la conexio feta en l'arxiu "db"
            $resultat = mysqli_query($conn, $query) or die(mysqli_error($conn));       

            // Es mira quants resultats ha tornat (només ha de coincidir 1 si és correcte, cap incorrecte)
            $count = mysqli_num_rows($resultat);

            // Guardem la ID del usuari que ha fet login
            $id = mysqli_fetch_array($resultat)['id'];

            if ($count == 1){
                sessio($conn, $query);
            } else {
                header("Location:../../index.php");
            }
        }
        
        // Guardem en la sessió les dades necessaries
        function sessio($conexio, $query) {
            session_start();
            $resultat = mysqli_query($conexio, $query) or die(mysqli_error($conexio));
               
            while($contingut = mysqli_fetch_array($resultat)){
				$_SESSION['id_usuari'] = $contingut['id'];
                $_SESSION['usuari'] = $contingut['usuari'];
                $_SESSION['tipus_user'] = $contingut['tipus_user'];
                $_SESSION['estat'] = 1; // estat actiu
            }
		   header("Location:../../perfil.php");

        }

    ?>
