      *----------------------------------------------------------------*
      *                                                                *
      *    - PROGRAMA    : SMPTSISW                                    *
      *    - OBJETIVO    : PROGRAMA DISTRIBUDOR, ENLAZA CON LA WEB Y   *
      *                    MANDA POR LA COLA EL MENSAJE Y ESPERA LA    *
      *                    RESPUESTA POR LA COLA                       *
      *    - DESCRIPCION : - ARRANCA EL TUXEDO LA TRANSACCION          *
      *                    - ENVIA  EL MENSAJE POR LA COLA             *
      *                    - RECIBE RESPUESTA POR LA COLA              *
      *                    - CONTESTA AL TUXEDO                        *
      *                                                                *
      *----------------------------------------------------------------*

      *----------------------------------------------------------------*
      *          I D E N T I F I C A T I O N   D I V I S I O N         *
      *          =============================================         *
      *----------------------------------------------------------------*

       IDENTIFICATION DIVISION.
      *-----------------------.
       PROGRAM-ID.      SMPTSISW.
       AUTHOR.        INDRA. 
       DATE-WRITTEN.    03 DE OCTUBRE DEL 2014.
       SECURITY.      PROGRAMA CONECTO WEB. 
                      
       ENVIRONMENT DIVISION.
      *--------------------.
       DATA DIVISION.
      *-------------.

       WORKING-STORAGE SECTION.
      *-----------------------.
      *----------------------------------------------------------------*
      *                   DEFINICION DATOS VARIABLES                   *
      *----------------------------------------------------------------*
       01  VARIABLES-WORKING.
        05 W-ENTRADA           PIC X(10).
        05 W-SALIDA            PIC X(1).
        05 PID-MENSAJE         PIC X(08).
           
      * CONTROL ERRORES
       01 WS-SMPC9998.
           COPY SMPC9999.

      *DATOS DE SALIDA
       01 SAL-COLA.
          05 SUBSTMA                 PIC X(02).
          05 SUAPL                   PIC X(02).
          05 CODIGOC                 PIC X(04).
          05 COD039                  PIC X(03).

       01 CT-CERO    PIC S9(8) VALUE 0.

      *----------------------------------------------------------------*
      * COPY DEL MENSAJE CONECTOR.                                     *
      *----------------------------------------------------------------*
        COPY SMPCCOLA.
        
      *----------------------------------------------------------------*
      *                   DEFINICION DATOS CONSTANTES                  *
      *----------------------------------------------------------------*

       01  CONSTANTES.

           05  CT-COLA-LECT             PIC  X(008) VALUE 'penvio01'.
           05  CT-COLA-ESCR             PIC  X(008) VALUE 'precep01'.

           05  CT-UNO                   PIC  9(001) VALUE 1.

           05  CT-MAX-COLA              PIC  9(002) VALUE 80.


      *-----------------------------------------------------------------
       LINKAGE SECTION.
      *-----------------------------------------------------------------
       01 DFHCOMMAREA.
           COPY SMPC0001.

 
      *----------------------------------------------------------------*
      *               P R O C E D U R E   D I V I S I O N              *
      *               ===================================              *
      *----------------------------------------------------------------*

       PROCEDURE DIVISION USING DFHCOMMAREA.
      *------------------------------------.
           DISPLAY "SMPTSISW"

           PERFORM 10000-INICIO-PROCESO

           PERFORM 30000-PROCESO

           PERFORM 90000-FIN-PROCESO.

      *----------------------------------------------------------------*
      *   INICIALIZACION DE  LAS VARIABLES DEL PROGRAMA.               *
      *----------------------------------------------------------------*

       10000-INICIO-PROCESO.
      *--------------------.
           INITIALIZE VARIABLES-WORKING
               REPLACING ALPHANUMERIC DATA BY SPACES
                          NUMERIC DATA BY ZEROS
           INITIALIZE WS-SMPC9998
               REPLACING ALPHANUMERIC DATA BY SPACES
                          NUMERIC DATA BY ZEROS
  
           MOVE C0001-MENSAJE TO W-ENTRADA.

      *----------------------------------------------------------------*
      * LECTURA DEL MENSAJE.                                           *
      * TRATAMIENTO DEL MENSAJE RECIBIDO.                              *
      *----------------------------------------------------------------*

       30000-PROCESO.
      *--------------.

	   PERFORM 31000-MANDAR-MENSAJE

           PERFORM 32000-RECIBIR-MENSAJE.

           PERFORM 33000-RELLENAR-SALIDA.

      *----------------------------------------------------------------*
      *  ESCRIBIMOS MENSAJE POR LA COLA                                *
      *----------------------------------------------------------------*
       31000-MANDAR-MENSAJE.
      *--------------------.
           MOVE CT-COLA-ESCR              TO COLA-NOM
           MOVE ZEROES                    TO COLA-ID
                                             COLA-TIME

           MOVE W-ENTRADA                  TO COLA-COMMAREA

           MOVE 10                         TO COLA-LENGTH
           DISPLAY "mandamos por la cola"

           CALL 'escribir_cola_id'   USING COLA-NOM-P
                                           COLA-ID-P
                                           COLA-TIME-P
                                           COLA-COMMAREA-P
                                           COLA-LENGTH-P
                                     RETURNING RESPCOL
           END-CALL

           DISPLAY "retorno de enviar por la cola: " RESPCOL
           IF RESPCOL > CT-CERO

              PERFORM 9990-TRATAMIENTO-ERROR

	   END-IF
           MOVE COLA-ID   TO PID-MENSAJE.


      *----------------------------------------------------------------*
      *  LEEMOS MENSAJE DE LA COLA                                     *
      *----------------------------------------------------------------*

       32000-RECIBIR-MENSAJE.
      *---------------------.

           MOVE CT-COLA-LECT               TO COLA-NOM
           MOVE '7'                       TO COLA-TIME
      ****** INDICA QUE LA COLA SE BLOQUEA HASTA QUE LEEI, PERO LO CAMBIAMOS
      **** POR AHORA, LUEGO DEBE ESTAR PARAMETRIZADO
           MOVE PID-MENSAJE                TO COLA-ID
           MOVE SPACES                     TO COLA-COMMAREA
           DISPLAY "espero mensaje"

           CALL 'leer_cola_cob_id' USING COLA-NOM-P
                                           COLA-ID-P
                                          COLA-TIME-P
                                          COLA-COMMAREA-P
                                          COLA-LENGTH-P
                              RETURNING   RESPCOL
           END-CALL
	 
           DISPLAY "retorno de esperar es:" RESPCOL
           IF RESPCOL > CT-CERO
             PERFORM 9990-TRATAMIENTO-ERROR

           ELSE
            MOVE COLA-COMMAREA   TO SAL-COLA

           END-IF.


       33000-RELLENAR-SALIDA.
      *----------------------.

           IF RESPCOL > CT-CERO

              PERFORM 9990-TRATAMIENTO-ERROR

           ELSE 
              SET C9999-CORRECTO    TO TRUE
              IF CODIGOC = '1814' AND COD039 = '800'
                 MOVE 1            TO C0001-MENSAJE
              ELSE
                 MOVE 0            TO C0001-MENSAJE
              END-IF
              MOVE 'SMPOSS01'                    TO C0001-NUM-MENSAJE
           END-IF.
      *----------------------------------------------------------------*
      *                                                                *
      *----------------------------------------------------------------*

       90000-FIN-PROCESO.
      *-----------------.

           MOVE C9999-DATOS-RETORNO           TO C0001-DATOS-RETORNO.
           GOBACK.

      *----------------------------------------------------------------*
      *      PARRAFOS GENERALES                                        *
      *----------------------------------------------------------------*
		
       9990-TRATAMIENTO-ERROR.
      *----------------------.
          SET C9999-ERROR           TO TRUE
          MOVE "000001"                  TO C9999-COD-AVISO1
          MOVE RESPCOL                   TO C9999-TEXT1-AVISO.
          MOVE "ERROR EN LA COMUNICACION" TO C9999-TEXT1-AVISO(9:)
          PERFORM 90000-FIN-PROCESO.

 
