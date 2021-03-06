#include "EventAction.hh"

 
EventAction::EventAction(Results* RE,RunAction* RA,Projectile* proj,DetectorConstruction* det):results(RE),run_action(RA),theProjectile(proj),theDetector(det)
{ 

  ionCollectionID=-1;
  CsICollectionID=-1;
  soa=16*4*sizeof(G4double);
  sov=16*4*sizeof(G4ThreeVector);
  soc=sizeof(TrackerCsIHitsCollection);
  soi=sizeof(TrackerIonHitsCollection);
  At=4;
  Zt=2;
  triggerAZ=0;
  SetTriggerParticleSing();
  CsIThreshold=0.;
  memset(GriffinCrystDisabled,0,sizeof(GriffinCrystDisabled));
  memset(CsIDisabled,0,sizeof(CsIDisabled));
  for(int i=0;i<NCsISph;i++){
    CsIWeight[i]=1.0;
  }
}


EventAction::~EventAction()
{
  ;
}

void EventAction::BeginOfEventAction(const G4Event*)
{
 
  G4SDManager * SDman = G4SDManager::GetSDMpointer();

  if(ionCollectionID<0)
    ionCollectionID=SDman->GetCollectionID("ionCollection");

  if(CsICollectionID<0)
    CsICollectionID=SDman->GetCollectionID("CsICollection");
 
  memset(GriffinCrystEnergyDet,0,soa);
  memset(GriffinCrystWeightDet,0,soa);
  memset(GriffinCrystPosDet,0,sov);
  GriffinFold=0;

  // G4cout<<"+++++ Begin of event "<<evt->GetEventID()<<G4endl;

 }


 
void EventAction::EndOfEventAction(const G4Event* evt)
{
  // G4cout<<"+++++ End of event "<<evt->GetEventID()<<G4endl;
  
  //increment event counter
  numEvents++;

  G4int evtNb=evt->GetEventID();

  if (evtNb%100 == 0) 
    {
      gettimeofday(&tc,NULL);
      ts=run_action->GetStartTime();
      timersub(&tc,&ts,&td);
      t=td.tv_sec;
      rate=(float)evtNb/t;
      G4cout<<" Number of processed events "<<std::setw(9)<<evtNb<<" in "<<std::setw(9)<<t<<" sec. at "<<std::fixed<<std::setw(9)<<std::setprecision(2)<<rate<<" events per second\r"<<std::flush;

  }

  one=1;
  eventTrigger=1;
  G4int Np;

  TrackerCsIHitsCollection* CsI;
  TrackerIonHitsCollection* HI;

  /*numP=run_action->GetNumberOfProtons();
  numN=run_action->GetNumberOfNeutrons();
  numA=run_action->GetNumberOfAlphas();*/
  
  int numDetHits=0;
  
  if(evt->GetHCofThisEvent()!=NULL) 
    {  
      CsI=(TrackerCsIHitsCollection*)(evt->GetHCofThisEvent()->GetHC(CsICollectionID)); 
      HI=(TrackerIonHitsCollection*)(evt->GetHCofThisEvent()->GetHC(ionCollectionID));
      Np=CsI->entries();
      //G4cout<<" Number of entries: "<<Np<<G4endl;
      //G4bool allHitsInOne=true;

      // CsI trigger
      if(Np>0) 
	      {
	      
	      	//nullify hits in disabled CsI detectors
	      	for(int i=0;i<Np;i++)
          	if(CsIDisabled[(*CsI)[i]->GetId()-1]==1)
	          	(*CsI)[i]->SetDisabled();
	        
	      
	        G4double partECsI[NCsISph]; //total energy deposit of particle in each detector
          G4double partACsI[NCsISph]; //A of particle in each detector
          G4double partZCsI[NCsISph]; //Z of particle in each detector
	        memset(partECsI,0,sizeof(partECsI));
	        for(int i=0;i<Np;i++)
	        	if((*CsI)[i]->GetDisabled()==0)
			        {
		            //G4cout<<" Hit detector: "<<(*CsI)[i]->GetId()<<G4endl;
		            if((CsIIDTrigger==0)||(((*CsI)[i]->GetId()-1)==CsIIDTrigger)) //CsI detector ID trigger
                  {
                    if(triggerAZ==1) // user defined particle trigger
                      {
                        if((*CsI)[i]->GetA()==At) // user defined particle trigger
                          if((*CsI)[i]->GetZ()==Zt)
                            {
                              partECsI[(*CsI)[i]->GetId()-1]+=(*CsI)[i]->GetKE();
                              partACsI[(*CsI)[i]->GetId()-1]=(*CsI)[i]->GetA();
                              partZCsI[(*CsI)[i]->GetId()-1]=(*CsI)[i]->GetZ();
                              //printf("particle A=%i Z=%i   CsI partial energy deposit is %9.3f in detector ID %i \n",(*CsI)[i]->GetA(),(*CsI)[i]->GetZ(),partECsI[(*CsI)[i]->GetId()-1],(*CsI)[i]->GetId());
                            }
                      }
                    else if(triggerAZ == 2) //user defined particle trigger range
                      {
                        if( ((*CsI)[i]->GetA()>=Atmin)&&((*CsI)[i]->GetA()<=Atmax) ) // user defined particle trigger range
                          if( ((*CsI)[i]->GetZ()>=Ztmin)&&((*CsI)[i]->GetZ()<=Ztmax) )
                            {
                              partECsI[(*CsI)[i]->GetId()-1]+=(*CsI)[i]->GetKE();
                              partACsI[(*CsI)[i]->GetId()-1]=(*CsI)[i]->GetA();
                              partZCsI[(*CsI)[i]->GetId()-1]=(*CsI)[i]->GetZ();
                              //printf("particle A=%i Z=%i   CsI partial energy deposit is %9.3f in detector ID %i \n",(*CsI)[i]->GetA(),(*CsI)[i]->GetZ(),partECsI[(*CsI)[i]->GetId()-1],(*CsI)[i]->GetId());
                            }
                      }
                    else // trigger on any charged particle
                      {
                        if((*CsI)[i]->GetA()>0)
                          {
                            partECsI[(*CsI)[i]->GetId()-1]+=(*CsI)[i]->GetKE();
                            partACsI[(*CsI)[i]->GetId()-1]=(*CsI)[i]->GetA();
                              partZCsI[(*CsI)[i]->GetId()-1]=(*CsI)[i]->GetZ();
                            //printf("particle A=%i Z=%i   CsI partial energy deposit is %9.3f in detector ID %i \n",(*CsI)[i]->GetA(),(*CsI)[i]->GetZ(),partECsI[(*CsI)[i]->GetId()-1],(*CsI)[i]->GetId());
                          }
                      }
                  }
			        }
          
          //determine the number of CsI hits 
          if(theDetector->usingCsIBall())
          	{
				      for(int i=0;i<NCsISph;i++)
				      	if(CsIDisabled[i]==0) //check if detector is disabled
							    if(partECsI[i]>=CsIThreshold)
                    {
                      numDetHits+=1.0*CsIWeight[i];
                      if((partACsI[i]==4)&&(partZCsI[i]==2))
                        numACsIHits+=1.0*CsIWeight[i];
                      if((partACsI[i]==1)&&(partZCsI[i]==1))
                        numPCsIHits+=1.0*CsIWeight[i];
                    }
							      
					  }
	        else
	        	{
	        		for(int i=0;i<NCsI;i++)
	        			if(CsIDisabled[i]==0) //check if detector is disabled
							    if(partECsI[i]>=CsIThreshold)
                    {
                      numDetHits+=1.0*CsIWeight[i];
                      if((partACsI[i]==4)&&(partZCsI[i]==2))
                        numACsIHits+=1.0*CsIWeight[i];
                      if((partACsI[i]==1)&&(partZCsI[i]==1))
                        numPCsIHits+=1.0*CsIWeight[i];
                    }
							      
	        	}

          //increment the counter for number of CsI hits (before triggering)
          numCsIhits+=numDetHits;
	        
	        //set entries in detectors below energy threshold to 0
	        for(int i=0;i<Np;i++)
            if((partECsI[(*CsI)[i]->GetId()-1]<CsIThreshold))
	            (*CsI)[i]->SetKE(0.0);
	        
	        //CsI singles trigger
	        if(numDetHits>0)  
	          eventTrigger|=(one<<10);
	        if(numDetHits==1)  
	          eventTrigger|=(one<<15);

          //particle-particle coincidence trigger
          //ie. particle hits in two or more different detectors
          if(numDetHits>1)
            eventTrigger|=(one<<11);

          if((eventTrigger&(one<<10))&&(eventTrigger&(one<<11)))
            eventTrigger|=(one<<12);
	  
          /*printf("recoil CsI total energy deposit is %9.3f\n",rECsI);
          printf("proj   CsI total energy deposit is %9.3f\n",pECsI);
          printf("CsI eventTrigger is %d\n",eventTrigger);
          getc(stdin);*/ 
	      } // end CsI trigger
	      
     
  
      // HPGe trigger
      if(GriffinFold>0)
        eventTrigger|=(one<<1);
      // end HPGe trigger
      
      //particle-particle coincidence AND gamma singles trigger
      if((eventTrigger&(one<<12))&&(eventTrigger&(one<<1)))
        eventTrigger|=(one<<13);
      
      //2+ unsupressed Griffin cores and 2 CsI (Nov 2013 S1232 trigger)  
      if((eventTrigger&(one<<12))&&(GriffinFold>1))
        eventTrigger|=(one<<14);

      /*printf("HPGe fold is %d\n",GriffinFold);
      for(testTrigger=1;testTrigger<=14;testTrigger++)
        {
          if(eventTrigger&(one<<testTrigger))
            G4cout<<"Event fulfills trigger condition "<<(int)testTrigger<<G4endl;
        }*/
  
      if(eventTrigger&(one<<setTrigger))
        {
          //increment triggered event and CsI hit counters
          numTriggeredEvents++;
          numTriggeredCsIHits+=numDetHits;

          /*printf("CsI fold is %d\n",numDetHits);
          printf("HPGe fold is %d\n",GriffinFold);
          for(testTrigger=1;testTrigger<=15;testTrigger++)
            {
              if(eventTrigger&(one<<testTrigger))
                G4cout<<"Event fulfills trigger condition "<<(int)testTrigger<<G4endl;
            }*/
        
	        if(GriffinFold>0)
	          for(G4int det=0;det<16;det++)
	            for(G4int cry=0;cry<4;cry++)
	              if( GriffinCrystEnergyDet[det][cry]>0)
		              GriffinCrystPosDet[det][cry]/=GriffinCrystEnergyDet[det][cry];
	        results->FillTree(evtNb,HI,CsI,GriffinCrystWeightDet,GriffinCrystEnergyDet,GriffinCrystPosDet,GriffinCrystTimeDet);
          //G4cout<<"Event fulfills trigger condition "<<setTrigger<<G4endl;
        }

    }
    // getc(stdin);
}
//*********************************************************************//
void EventAction::AddGriffinCrystDet(G4double de, G4double w, G4ThreeVector pos, G4int det, G4int cry, G4double t) 
{
  if(GriffinCrystDisabled[det][cry]==1)
    return;//don't do anything if the crystal is disabled
    
  //G4cout<<" Detector "<<det<<" crystal "<<cry<<" weight "<<w<<G4endl;
  if(GriffinCrystWeightDet[det][cry]==0.)
    {
      GriffinCrystWeightDet[det][cry]=w;
      GriffinCrystEnergyDet[det][cry]=de;
      GriffinCrystPosDet[det][cry]=de*pos;
      GriffinCrystTimeDet[det][cry]=t;
      GriffinFold++;
      return;
    }

  if(GriffinCrystWeightDet[det][cry]==w)
    {
      GriffinCrystEnergyDet[det][cry]+=de;
      GriffinCrystPosDet[det][cry]+= de*pos;
    }
  else
    {
      G4cout<<G4endl;
      G4cout<<" Bad weight: detector "<<det<<" crystal "<<cry<<" weight "<<w<<" old weight "<<GriffinCrystWeightDet[det][cry]<<G4endl;
      G4cout<<" Terminating execution "<<G4endl;
      G4cout<<" Find the reason, make a fix "<<G4endl;
      G4cout<<" Resist a temptation to patch or swipe under a carpet "<<G4endl;
      G4cout<<" May the force be with you, press ENTER "<<G4endl;
      getc(stdin);
      exit(0);
    }
}
//---------------------------------------------------------
void EventAction::setTriggerA(G4int Ain)
{

  At=Ain;
  triggerAZ=1;
  G4cout<<"----> A of the particle to trigger on set to  "<<At<< G4endl;
  
}
//---------------------------------------------------------
void EventAction::setTriggerZ(G4int Zin)
{

  Zt=Zin;
  triggerAZ=1;
  G4cout<<"----> Z of the particle to trigger on set to  "<<Zt<< G4endl;
  
}
//---------------------------------------------------------
void EventAction::setTriggerARange(G4int min, G4int max)
{

  Atmin=min;
  Atmax=max;
  triggerAZ=2;
  G4cout<<"----> A of the particle to trigger on set to range from " << min << " to " << max << "." << G4endl;
  
}
//---------------------------------------------------------
void EventAction::setTriggerZRange(G4int min, G4int max)
{

  Ztmin=min;
  Ztmax=max;
  triggerAZ=2;
  G4cout<<"----> Z of the particle to trigger on set range from " << min << " to " << max << "." << G4endl;
  
}
//---------------------------------------------------------
void EventAction::reportTriggers()
{
  G4cout<<"TRIGGER REPORT"<<G4endl<<"--------------"<<G4endl;
  G4cout<<"Number of events: "<<numEvents<< G4endl;
  G4cout<<"Number of triggered events: "<<numTriggeredEvents<< G4endl;
  G4cout<<"Total number of CsI hits in all events: "<<numCsIhits<< G4endl;
  G4cout<<"Total number of CsI proton hits in all events: "<<numPCsIHits<< G4endl;
  G4cout<<"Total number of CsI alpha hits in all events: "<<numACsIHits<< G4endl;
  G4cout<<"Total number of CsI hits in triggered events: "<<numTriggeredCsIHits<< G4endl;
}
