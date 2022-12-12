void RCX_Task_setup(void *arg); 
/*
om de taak aan te maken voer je uiteraard zijn setup uit voor de scedular.
plak:
RCX_Task_setup();           
*/

void RCX_Task_function(void *arg);

/*
deze functie zal je echter zelf moeten schrijven
als je de functie in de gegenereerde sectie schrijft moet je wel weten dat:
bij het opnieuw maken van de hardware de functie terug weg is:

een plak voorbeeldje:

void RCX_Task_function(void *arg)
{
    (void)arg;
    
        while(1) {
        vTaskDelay(600);
        }
}


*/