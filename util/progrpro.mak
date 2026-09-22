executables:$(EXEC)/arranqueI \
    $(EXEC)/arranqueA \
	$(EXEC)/arranqueW \
	$(EXEC)/paradaI \
	$(EXEC)/paradaA \
	$(EXEC)/paradaW \
	$(EXEC)/superviI \
    $(EXEC)/superviA \
	$(EXEC)/superviW \
	$(EXEC)/penvio30 \
	$(EXEC)/precep30 \
	$(EXEC)/rtratami \
	$(EXEC)/cl3001re \
	$(EXEC)/cl3002re \
	$(EXEC)/cl3003re \
	$(EXEC)/cl3004re \
	$(EXEC)/cl3001en \
	$(EXEC)/cl3002en \
	$(EXEC)/cl3003en \
	$(EXEC)/cl3004en \
	$(EXEC)/penvio03 \
	$(EXEC)/precep03 \
	$(EXEC)/ptimer03 \
	$(EXEC)/ptimer30 \
	$(EXEC)/pmanej03 \
	$(EXEC)/rdif0301 \
	$(EXEC)/rdif3001 \
	$(EXEC)/rdif3002 \
	$(EXEC)/rdif3003 \
	$(EXEC)/rdif3004 \
	$(EXEC)/penvio01 \
	$(EXEC)/precep01 \
	$(EXEC)/precep40 \
	$(EXEC)/penvio40 \
	$(EXEC)/cl4002re \
	$(EXEC)/cl4002en \
	$(EXEC)/ptimer40 \
    $(EXEC)/rdif4002 \
    $(EXEC)/penvio02 \
    $(EXEC)/rtrataan \
    $(EXEC)/penvio05 \
 	$(EXEC)/precep05 \
    $(EXEC)/ptimer05 \
 	$(EXEC)/pmanej05 \
 	$(EXEC)/rdif0501 \
    $(EXEC)/precep02 \
 	$(EXEC)/precep20 \
 	$(EXEC)/penvio20 \
 	$(EXEC)/cl2002re \
 	$(EXEC)/cl2002en \
 	$(EXEC)/ptimer20 \
    $(EXEC)/rdif2002 \
	$(EXEC)/penvio04 \
    $(EXEC)/rtratawp \
    $(EXEC)/penvio07 \
 	$(EXEC)/precep07 \
    $(EXEC)/ptimer07 \
 	$(EXEC)/pmanej07 \
 	$(EXEC)/rdif0701 \
    $(EXEC)/precep04 \
 	$(EXEC)/precep10 \
 	$(EXEC)/penvio10 \
 	$(EXEC)/cl1002re \
 	$(EXEC)/cl1002en \
 	$(EXEC)/ptimer10 \
    $(EXEC)/rdif1002 \
	$(EXEC)/ControlHSM \
	$(EXEC)/EnviaComandosHSM
## PRJ0258542 - para compilar con todo el paquete	
##	$(EXEC)/pmanej40 
##	$(EXEC)/cl4004en \
##	$(EXEC)/cl4004re \
## $(EXEC)/rdif4002 \

#LN6 es sisma, es decir, todos los obj
#PROGRAMA=$(EXEC)/arranque 

##$(EXEC)/arranque: $(OBJ)/arranque.o $(OBJ)/filefun.o $(LIBNET)
##	$(COMPILER) -m64 -o $(EXEC)/arranque $(OBJ)/arranque.o  $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
##	chmod 777  $(EXEC)/arranque
## PRJ0258542 - para compilar con todo el paquete
$(EXEC)/ControlHSM: $(OBJ)/ControlHSM.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/ControlHSM $(OBJ)/ControlHSM.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/ControlHSM

$(EXEC)/EnviaComandosHSM: $(OBJ)/EnviaComandosHSM.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/EnviaComandosHSM $(OBJ)/EnviaComandosHSM.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/EnviaComandosHSM

$(EXEC)/arranqueI: $(OBJ)/arranque.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/arranqueI $(OBJ)/arranque.o  $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777  $(EXEC)/arranqueI

$(EXEC)/arranqueA:  
	cp $(EXEC)/arranqueI $(EXEC)/arranqueA

$(EXEC)/arranqueW:  
	cp $(EXEC)/arranqueI $(EXEC)/arranqueW


##$(EXEC)/parada: $(OBJ)/parada.o $(OBJ)/filefun.o $(LIBNET)
##	$(COMPILER) -m64 -o $(EXEC)/parada $(OBJ)/parada.o  $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
##	chmod 777 $(EXEC)/parada

$(EXEC)/paradaI: $(OBJ)/parada.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/paradaI $(OBJ)/parada.o  $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/paradaI
    
$(EXEC)/paradaA: $(OBJ)/parada.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/paradaA $(OBJ)/parada.o  $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/paradaA

$(EXEC)/paradaW: $(OBJ)/parada.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/paradaW $(OBJ)/parada.o  $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/paradaW

$(EXEC)/rdif0301: $(OBJ)/rdifxxxx.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/rdif0301 $(OBJ)/rdifxxxx.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/rdif0301

$(EXEC)/rdif3001:  
	cp $(EXEC)/rdif0301 $(EXEC)/rdif3001
$(EXEC)/rdif3002: 
	cp $(EXEC)/rdif0301 $(EXEC)/rdif3002
$(EXEC)/rdif3003: 
	cp $(EXEC)/rdif0301 $(EXEC)/rdif3003
$(EXEC)/rdif3004: 
	cp $(EXEC)/rdif0301 $(EXEC)/rdif3004 
$(EXEC)/rdif4002: 
	cp $(EXEC)/rdif0301 $(EXEC)/rdif4002
$(EXEC)/rdif0501: 
	cp $(EXEC)/rdif0301 $(EXEC)/rdif0501
$(EXEC)/rdif2002: 
	cp $(EXEC)/rdif0301 $(EXEC)/rdif2002
$(EXEC)/rdif1002: 
	cp $(EXEC)/rdif0301 $(EXEC)/rdif1002
$(EXEC)/rdif0701: 
	cp $(EXEC)/rdif0301 $(EXEC)/rdif0701


$(EXEC)/penvio30: $(OBJ)/penvioRed.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/penvio30 $(OBJ)/penvioRed.o $(OBJ)/filefun.o $(OBJ)/conexhsm.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/penvio30

$(EXEC)/precep30: $(OBJ)/precep_red.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/precep30 $(OBJ)/precep_red.o $(OBJ)/filefun.o $(OBJ)/conexhsm.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/precep30


$(EXEC)/penvio03: $(OBJ)/penvioreso.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -g -o $(EXEC)/penvio03 $(OBJ)/penvioreso.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/penvio03

$(EXEC)/precep03: $(OBJ)/precep_reso.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/precep03 $(OBJ)/precep_reso.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/precep03

$(EXEC)/cl3001re: $(OBJ)/clxxxxre.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -g -o $(EXEC)/cl3001re $(OBJ)/clxxxxre.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/cl3001re

$(EXEC)/cl3001en: $(OBJ)/clxxxxen.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/cl3001en $(OBJ)/clxxxxen.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/cl3001en

$(EXEC)/cl3002re:
	cp $(EXEC)/cl3001re $(EXEC)/cl3002re

$(EXEC)/cl3003re:
	cp $(EXEC)/cl3001re $(EXEC)/cl3003re

$(EXEC)/cl3004re:
	cp $(EXEC)/cl3001re $(EXEC)/cl3004re

$(EXEC)/cl3002en:
	cp $(EXEC)/cl3001en $(EXEC)/cl3002en

$(EXEC)/cl3003en:
	cp $(EXEC)/cl3001en $(EXEC)/cl3003en

$(EXEC)/cl3004en:
	cp $(EXEC)/cl3001en $(EXEC)/cl3004en    
    
$(EXEC)/cl4002re:
	cp $(EXEC)/cl3001re $(EXEC)/cl4002re
##$(EXEC)/cl4004re:
##	cp $(EXEC)/cl3001re $(EXEC)/cl4004re
$(EXEC)/cl4002en:
	cp $(EXEC)/cl3001en $(EXEC)/cl4002en
##$(EXEC)/cl4004en:
##	cp $(EXEC)/cl3001en $(EXEC)/cl4004en  

$(EXEC)/cl2002re:
	cp $(EXEC)/cl3001re $(EXEC)/cl2002re
$(EXEC)/cl2002en:
	cp $(EXEC)/cl3001en $(EXEC)/cl2002en
	
$(EXEC)/cl1002re:
	cp $(EXEC)/cl3001re $(EXEC)/cl1002re
$(EXEC)/cl1002en:
	cp $(EXEC)/cl3001en $(EXEC)/cl1002en

$(EXEC)/penvio01: $(OBJ)/penvio01.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/penvio01 $(OBJ)/penvio01.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/penvio01

$(EXEC)/precep01: $(OBJ)/precep01.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/precep01 $(OBJ)/precep01.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/precep01

$(EXEC)/rtratami: $(OBJ)/rtratami.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/rtratami $(OBJ)/rtratami.o $(OBJ)/filefun.o $(OBJ)/conexhsm.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/rtratami

$(EXEC)/ptimer03: $(OBJ)/ptimerxx.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/ptimer03 $(OBJ)/ptimerxx.o $(OBJ)/filefun.o  -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/ptimer03

$(EXEC)/ptimer30: $(OBJ)/ptimerxx.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/ptimer30 $(OBJ)/ptimerxx.o $(OBJ)/filefun.o  -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/ptimer30
    
$(EXEC)/ptimer40: $(OBJ)/ptimerxx.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/ptimer40 $(OBJ)/ptimerxx.o $(OBJ)/filefun.o  -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/ptimer40

##$(EXEC)/supervis: $(OBJ)/supervis.o $(OBJ)/filefun.o $(LIBNET)
##	$(COMPILER) -m64 -o $(EXEC)/supervis $(OBJ)/supervis.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
##	chmod 777 $(EXEC)/supervis
    
$(EXEC)/superviI: $(OBJ)/supervisInversis.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/superviI $(OBJ)/supervisInversis.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/superviI

$(EXEC)/superviA: $(OBJ)/supervisAnbank.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/superviA $(OBJ)/supervisAnbank.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/superviA
	
$(EXEC)/superviW: $(OBJ)/supervisWpb.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/superviW $(OBJ)/supervisWpb.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/superviW
	
$(EXEC)/pmanej03: $(OBJ)/pmanej03.o $(OBJ)/filefun.o $(LIBNET)                                                           
	$(COMPILER) -m64 -o $(EXEC)/pmanej03 $(OBJ)/pmanej03.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/pmanej03                                                                                                      
	                                                                                                                                
$(EXEC)/precep40: $(OBJ)/precep_red.o $(OBJ)/filefun.o $(LIBNET)                                                          
	$(COMPILER) -m64 -o $(EXEC)/precep40 $(OBJ)/precep_red.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/precep40                                                                                             
                                                                                                                           
$(EXEC)/penvio40: $(OBJ)/penvioRed.o $(OBJ)/filefun.o $(LIBNET)                                                           
	$(COMPILER) -m64 -o $(EXEC)/penvio40 $(OBJ)/penvioRed.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/penvio40           

$(EXEC)/penvio02: $(OBJ)/penvio01.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/penvio02 $(OBJ)/penvio01.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/penvio02 

$(EXEC)/rtrataan: $(OBJ)/rtratami.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/rtrataan $(OBJ)/rtratami.o $(OBJ)/filefun.o $(OBJ)/conexhsm.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/rtrataan  
    
$(EXEC)/penvio05: $(OBJ)/penvioreso.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -g -o $(EXEC)/penvio05 $(OBJ)/penvioreso.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/penvio05
    
$(EXEC)/precep05: $(OBJ)/precep_reso.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/precep05 $(OBJ)/precep_reso.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/precep05

$(EXEC)/ptimer05: $(OBJ)/ptimerxx.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/ptimer05 $(OBJ)/ptimerxx.o $(OBJ)/filefun.o  -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/ptimer05

$(EXEC)/pmanej05: $(OBJ)/pmanej03.o $(OBJ)/filefun.o $(LIBNET)                                                           
	$(COMPILER) -m64 -o $(EXEC)/pmanej05 $(OBJ)/pmanej03.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/pmanej05   

$(EXEC)/precep02: $(OBJ)/precep01.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/precep02 $(OBJ)/precep01.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/precep02  

$(EXEC)/precep20: $(OBJ)/precep_red.o $(OBJ)/filefun.o $(LIBNET)                                                          
	$(COMPILER) -m64 -o $(EXEC)/precep20 $(OBJ)/precep_red.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/precep20   

$(EXEC)/penvio20: $(OBJ)/penvioRed.o $(OBJ)/filefun.o $(LIBNET)                                                           
	$(COMPILER) -m64 -o $(EXEC)/penvio20 $(OBJ)/penvioRed.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/penvio20  
    
$(EXEC)/ptimer20: $(OBJ)/ptimerxx.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/ptimer20 $(OBJ)/ptimerxx.o $(OBJ)/filefun.o  -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/ptimer20    
 
$(EXEC)/penvio04: $(OBJ)/penvio01.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/penvio04 $(OBJ)/penvio01.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/penvio04 

$(EXEC)/rtratawp: $(OBJ)/rtratami.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/rtratawp $(OBJ)/rtratami.o $(OBJ)/filefun.o $(OBJ)/conexhsm.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/rtratawp  
    
$(EXEC)/penvio07: $(OBJ)/penvioreso.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -g -o $(EXEC)/penvio07 $(OBJ)/penvioreso.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/penvio07
    
$(EXEC)/precep07: $(OBJ)/precep_reso.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/precep07 $(OBJ)/precep_reso.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/precep07

$(EXEC)/ptimer07: $(OBJ)/ptimerxx.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/ptimer07 $(OBJ)/ptimerxx.o $(OBJ)/filefun.o  -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/ptimer07

$(EXEC)/pmanej07: $(OBJ)/pmanej03.o $(OBJ)/filefun.o $(LIBNET)                                                           
	$(COMPILER) -m64 -o $(EXEC)/pmanej07 $(OBJ)/pmanej03.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/pmanej07   

$(EXEC)/precep04: $(OBJ)/precep01.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/precep04 $(OBJ)/precep01.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/precep04  

$(EXEC)/precep10: $(OBJ)/precep_red.o $(OBJ)/filefun.o $(LIBNET)                                                          
	$(COMPILER) -m64 -o $(EXEC)/precep10 $(OBJ)/precep_red.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/precep10   

$(EXEC)/penvio10: $(OBJ)/penvioRed.o $(OBJ)/filefun.o $(LIBNET)                                                           
	$(COMPILER) -m64 -o $(EXEC)/penvio10 $(OBJ)/penvioRed.o $(OBJ)/filefun.o -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	chmod 777 $(EXEC)/penvio10  
    
$(EXEC)/ptimer10: $(OBJ)/ptimerxx.o $(OBJ)/filefun.o $(LIBNET)
	$(COMPILER) -m64 -o $(EXEC)/ptimer10 $(OBJ)/ptimerxx.o $(OBJ)/filefun.o  -L$(OBJ) -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	chmod 777 $(EXEC)/ptimer10
	
##$(OBJ)/arranque.o: $(FUEN1)/arranque.c $(INCLUD)/arranque.h
##	$(COMPILER) -m64 -c $(P) $(FUEN1)/arranque.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
##	mv arranque.o $(OBJ)
##	chmod 777  $(OBJ)/arranque.o
## PRJ0258542 - para compilar con todo el paquete
$(OBJ)/ControlHSM.o: $(FUEN1)/ControlHSM.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/ControlHSM.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	mv ControlHSM.o $(OBJ)
	chmod 777  $(OBJ)/ControlHSM.o
    
$(OBJ)/EnviaComandosHSM.o: $(FUEN1)/EnviaComandosHSM.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/EnviaComandosHSM.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	mv EnviaComandosHSM.o $(OBJ)
	chmod 777  $(OBJ)/EnviaComandosHSM.o

$(OBJ)/arranque.o: $(FUEN1)/arranque.c $(INCLUD)/arranque.h
	$(COMPILER) -m64 -c $(P) $(FUEN1)/arranque.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	mv arranque.o $(OBJ)
	chmod 777  $(OBJ)/arranque.o

##$(OBJ)/parada.o: $(FUEN1)/parada.c
##	$(COMPILER) -m64 -c $(P) $(FUEN1)/parada.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
##	mv parada.o $(OBJ)
##	chmod 777  $(OBJ)/parada.o

$(OBJ)/parada.o: $(FUEN1)/parada.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/parada.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv parada.o $(OBJ)
	chmod 777  $(OBJ)/parada.o

$(OBJ)/penvioRed.o: $(FUEN1)/penvioRed.c $(FUEN1)/ftradred.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/penvioRed.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv penvioRed.o $(OBJ)
	chmod 777  $(OBJ)/penvioRed.o

$(OBJ)/precep_red.o: $(FUEN1)/precep_red.c $(FUEN1)/ftradred.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/precep_red.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	mv precep_red.o $(OBJ)
	chmod 777  $(OBJ)/precep_red.o

$(OBJ)/penvioreso.o: $(FUEN1)/penvioreso.c $(FUEN1)/ftrad03.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/penvioreso.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv penvioreso.o $(OBJ)
	chmod 777  $(OBJ)/penvioreso.o

$(OBJ)/precep_reso.o: $(FUEN1)/precep_reso.c $(FUEN1)/ftrad03.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/precep_reso.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3) 
	mv precep_reso.o $(OBJ)
	chmod 777  $(OBJ)/precep_reso.o

$(OBJ)/clxxxxre.o: $(FUEN1)/clxxxxre.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/clxxxxre.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv clxxxxre.o $(OBJ)
	chmod 777  $(OBJ)/clxxxxre.o                                                    
                                                                                    
$(OBJ)/clxxxxen.o: $(FUEN1)/clxxxxen.c                                              
	$(COMPILER) -m64 -c $(P) $(FUEN1)/clxxxxen.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv clxxxxen.o $(OBJ)                                                            
	chmod 777  $(OBJ)/clxxxxen.o                                                    
                                                                                    
$(OBJ)/penvio01.o: $(FUEN1)/penvio01.c                                              
	$(COMPILER) -m64 -c $(P) $(FUEN1)/penvio01.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv penvio01.o $(OBJ)                                                            
	chmod 777  $(OBJ)/penvio01.o    

$(OBJ)/penvio02.o: $(FUEN1)/penvio01.c                                              
	$(COMPILER) -m64 -c $(P) $(FUEN1)/penvio01.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv penvio02.o $(OBJ)                                                            
	chmod 777  $(OBJ)/penvio02.o     

$(OBJ)/penvio04.o: $(FUEN1)/penvio01.c                                              
	$(COMPILER) -m64 -c $(P) $(FUEN1)/penvio01.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv penvio04.o $(OBJ)                                                            
	chmod 777  $(OBJ)/penvio04.o   
                                                                                    
$(OBJ)/precep01.o: $(FUEN1)/precep01.c                                              
	$(COMPILER) -m64 -c $(P) $(FUEN1)/precep01.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv precep01.o $(OBJ)                                                            
	chmod 777  $(OBJ)/precep01.o                                                    
                                                                                    
$(OBJ)/precep02.o: $(FUEN1)/precep01.c                                              
	$(COMPILER) -m64 -c $(P) $(FUEN1)/precep01.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv precep02.o $(OBJ)                                                            
	chmod 777  $(OBJ)/precep02.o   

$(OBJ)/precep04.o: $(FUEN1)/precep01.c                                              
	$(COMPILER) -m64 -c $(P) $(FUEN1)/precep01.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv precep04.o $(OBJ)                                                            
	chmod 777  $(OBJ)/precep04.o 	
                                                                                    
$(OBJ)/rtratami.o: $(FUEN1)/rtratami.c $(FUEN1)/ftrata??.c                          
	$(COMPILER) -m64 -g -c $(P) $(FUEN1)/rtratami.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv rtratami.o $(OBJ)
	chmod 777  $(OBJ)/rtratami.o
    
$(OBJ)/rtrataan.o: $(FUEN1)/rtratami.c $(FUEN1)/ftrata??.c                          
	$(COMPILER) -m64 -g -c $(P) $(FUEN1)/rtratami.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv rtrataan.o $(OBJ)
	chmod 777  $(OBJ)/rtrataan.o

$(OBJ)/rtratawp.o: $(FUEN1)/rtratami.c $(FUEN1)/ftrata??.c                          
	$(COMPILER) -m64 -g -c $(P) $(FUEN1)/rtratami.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv rtratawp.o $(OBJ)
	chmod 777  $(OBJ)/rtratawp.o

$(OBJ)/ptimerxx.o: $(FUEN1)/ptimerxx.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/ptimerxx.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv ptimerxx.o $(OBJ)
	chmod 777  $(OBJ)/ptimerxx.o

##$(OBJ)/supervis.o: $(FUEN1)/supervis.c
##	$(COMPILER) -m64 -c $(P) $(FUEN1)/supervis.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
##	mv supervis.o $(OBJ)
##	chmod 777 $(OBJ)/supervis.o

$(OBJ)/supervisInversis.o: $(FUEN1)/supervisInversis.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/supervisInversis.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv supervisInversis.o $(OBJ)
	chmod 777 $(OBJ)/supervisInversis.o
    
$(OBJ)/supervisAnbank.o: $(FUEN1)/supervisAnbank.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/supervisAnbank.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv supervisAnbank.o $(OBJ)
	chmod 777 $(OBJ)/supervisAnbank.o
	
$(OBJ)/supervisWpb.o: $(FUEN1)/supervisWpb.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/supervisWpb.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv supervisWpb.o $(OBJ)
	chmod 777 $(OBJ)/supervisWpb.o


$(OBJ)/rdifxxxx.o: $(FUEN1)/rdifxxxx.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/rdifxxxx.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv rdifxxxx.o $(OBJ)
	chmod 777  $(OBJ)/rdifxxxx.o 

$(OBJ)/pmanej03.o: $(FUEN1)/pmanej03.c
	$(COMPILER) -m64 -c $(P) $(FUEN1)/pmanej03.c -l$(LN6) -L$(LN2) -L$(LN7) -l$(LN3)
	mv pmanej03.o $(OBJ)
	chmod 777  $(OBJ)/pmanej03.o

	
#$(OBJ)/bfindedia.o: $(FUEN1)/bfindedia.c
##	$(COMPILER) -m64 -c $(P) $(FUEN1)/bfindedia.c
##	mv bfindedia.o $(OBJ)
##    chmod 777  $(OBJ)/bfindedia.o




