DROP DATABASE chemistbot;
CREATE DATABASE chemistbot;
USE chemistbot;

CREATE TABLE medicaments (
    id          INT(3)      NOT NULL,
    nom         VARCHAR(32) NOT NULL,
    quantitat   INT(3)      NOT NULL,
    cartutx    	INT(3)      NOT NULL,
    
    PRIMARY KEY (id)
);

CREATE TABLE usuaris (
    id          INT(2)          NOT NULL,
    usuari      VARCHAR(32)     NOT NULL UNIQUE,
    password    VARCHAR(32)     NOT NULL,
    tipus       INT(1)          NOT NULL,
    
    nom         VARCHAR(32)     NOT NULL,
    pes         int(3),
    altura      float(3,2),
    edat        int(3),
        
    PRIMARY KEY (id)
);

CREATE TABLE horaris (
    id          INT(3)      NOT NULL,
    id_med      INT(3)      NOT NULL,
    id_user     INT(2)      NOT NULL,
    dia_setmana INT(1)      NOT NULL,
    hora        VARCHAR(10) NOT NULL,
	quantitat 	INT(3)		NOT NULL,
    
    PRIMARY KEY (id),
    FOREIGN KEY (id_med) REFERENCES medicaments(id) ON DELETE CASCADE,
    FOREIGN KEY (id_user) REFERENCES usuaris(id) ON DELETE CASCADE
);


/* TEST */

/** MEDICAMENTS **/
INSERT INTO medicaments VALUES (1, 'Medicament A', 2, 2);
INSERT INTO medicaments VALUES (2, 'Medicament B', 2, 2);

/** USUARIS **/
INSERT INTO usuaris VALUES (1, 'admin', 'admin', 0, 'Admin', NULL, NULL, NULL);

/** HORARIS **/
INSERT INTO horaris VALUES (1, 1, 2, 1, '10:15', 2);
INSERT INTO horaris VALUES (2, 1, 3, 2, '21:00', 2);




