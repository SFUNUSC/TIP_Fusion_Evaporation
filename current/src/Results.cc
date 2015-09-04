#include "Results.hh"

Results::Results(Projectile* proj):theProjectile(proj)
//Results::Results(Projectile* proj,Recoil* rec,DetectorConstruction* det):theProjectile(proj),theRecoil(rec),theDetector(det)
{
  h=NULL;
  g=NULL;  
  c=NULL;
  TreeCreate();  
  IonFill=0;
  soh=sizeof(rHit);
  soi=sizeof(gun);
  sos=sizeof(stat);
  memset(CP,0,sizeof(CP));

  // HPGe FWHM response parameters
  F=1.25;
  G=2.00;
  H=0.00;

  // Energy calibration parameters
  A=0.0;
  B=1.0;
  C=0;

}

Results::~Results()
{
tree->Delete();
}
//---------------------------------------------------------
void Results::SetupRun(G4int DA, G4int DZ)
{
  Ap=theProjectile->getA();
  Zp=theProjectile->getZ();

  Ar=Ap+DA;
  Zr=Zp+DZ;
  
}
//---------------------------------------------------------
void Results::TreeCreate()
{
 if(tree==NULL)
    {

      tree= new TTree("tree","tree");
      tree->Branch("Gfold",&Gfold,"Gfold/I");
      tree->Branch("GId",GId,"GId[Gfold]/I");
      tree->Branch("GSeg",GSeg,"GSeg[Gfold]/I");
      tree->Branch("Gx",Gx,"Gx[Gfold]/D");
      tree->Branch("Gy",Gy,"Gy[Gfold]/D");
      tree->Branch("Gz",Gz,"Gz[Gfold]/D");
      tree->Branch("GE",GE,"GE[Gfold]/D");
      tree->Branch("GW",GW,"GW[Gfold]/D");
      tree->Branch("stat",&stat,"evNb/I:Ap/I:Zp/I");

      tree->Branch("projGun",&gun,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("projTargetIn",&pTIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("projReactionIn",&pRIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
        
      tree->Branch("recReactionOut",&rROut,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("recBackingIn",&rBIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("recBackingOut",&rBOut,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("recDec",&rDec,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");

      /*tree= new TTree("tree","tree");
      tree->Branch("Gfold",&Gfold,"Gfold/I");
      tree->Branch("GId",GId,"GId[Gfold]/I");
      tree->Branch("GSeg",GSeg,"GSeg[Gfold]/I");
      tree->Branch("Gx",Gx,"Gx[Gfold]/D");
      tree->Branch("Gy",Gy,"Gy[Gfold]/D");
      tree->Branch("Gz",Gz,"Gz[Gfold]/D");
      tree->Branch("GE",GE,"GE[Gfold]/D");
      tree->Branch("GW",GW,"GW[Gfold]/D");
      tree->Branch("stat",&stat,"evNb/I:Ap/I:Zp/I:Ar/I:Zr/I");

      // can include path and eloss for any IonInf but they are all == 0
      tree->Branch("pHit",&pHit,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:Id/D:r/D");
      tree->Branch("rHit",&rHit,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:Id/D:r/D");
      tree->Branch("projGun",&gun,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("projBackingIn",&pBIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("projTargetIn",&pTIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("projReactionIn",&pRIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("projReactionOut",&pROut,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("projTargetOut",&pTOut,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("projDegraderIn",&pDIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("projDegraderOut",&pDOut,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("projDec",&pDec,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      
      tree->Branch("recReactionOut",&rROut,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("recTargetOut",&rTOut,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("recDegraderIn",&rDIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("recDegraderIn",&rDIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");
      tree->Branch("recDegraderTrack",&rDTrack,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D"); // only the relavent stuff?
      tree->Branch("recDec",&rDec,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D");*/
   }

}
//---------------------------------------------------------
void Results::TreeClear()
{
  tree->Delete("all");
  tree=NULL;
  TreeCreate();
}
//---------------------------------------------------------
void Results::TreeReport()
{
  tree->Print(); 
}
//---------------------------------------------------------
void Results::TreeView()
{
  gStyle->SetPalette(1);
  tree->StartViewer(); 
}
//---------------------------------------------------------
void Results::TreeSave(G4String name)
{
  TDirectory *dir=new TDirectory();
  dir=gDirectory;
  TFile f(name,"recreate");
  f.cd();
  tree->AutoSave();
  tree->SetDirectory(dir);
  f.Close();
  dir->cd();
  G4cout<<"Tree of simulated parameters saved in file "<<name<<G4endl;
  
}
//---------------------------------------------------------
void Results::TreeRead(G4String)
{
 
}

//---------------------------------------------------------
void Results::TreeAdd(G4String )
{

 

}
//---------------------------------------------------------
void Results::FillTree(G4int evtNb, TrackerIonHitsCollection* IonCollection,TrackerCsIHitsCollection* CsICollection,G4double gw[GN][GS],G4double ge[GN][GS],G4ThreeVector gp[GN][GS])
{

 G4int Nt=IonCollection->entries();

 memset(&stat,0,sos);
 memset(&rHit,0,soh);
 memset(&pHit,0,soh);
 memset(&gun,0,soi);
 memset(&pBIn,0,soi);
 memset(&pTIn,0,soi);
 memset(&pRIn,0,soi);
 memset(&pROut,0,soi);
 memset(&pTOut,0,soi);
 memset(&pDIn,0,soi);
 memset(&pDOut,0,soi);
 memset(&pDec,0,soi);
 memset(&rROut,0,soi);
 memset(&rTOut,0,soi);
 memset(&rDIn,0,soi);
 memset(&rDOut,0,soi);
 memset(&rDec,0,soi);
 memset(&rDTrack,0,soi);

 if(Nt>0) 
   {
      
     stat.evNb=evtNb;
     stat.Ap=Ap;
     stat.Zp=Zp;
 
     for(Int_t i=0;i<Nt;i++)
       switch((*IonCollection)[i]->GetFlag())
	 {
	 case GUN_FLAG:
	   if((*IonCollection)[i]->GetA()==Ap)
	     if((*IonCollection)[i]->GetZ()==Zp)
	       {
		 gun.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 gun.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 gun.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 gun.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 gun.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 gun.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 gun.b=(*IonCollection)[i]->GetBeta();
		 gun.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 gun.w=(*IonCollection)[i]->GetWeight();

	       }
	   break;

	 case TARGET_IN_FLAG:
	   if((*IonCollection)[i]->GetA()==Ap)
	     if((*IonCollection)[i]->GetZ()==Zp)
	       {
		 pTIn.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 pTIn.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 pTIn.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 pTIn.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 pTIn.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 pTIn.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 pTIn.b=(*IonCollection)[i]->GetBeta();
		 pTIn.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 pTIn.w=(*IonCollection)[i]->GetWeight();
	       }
	   break;
	 case BACKING_IN_FLAG:
	   if((*IonCollection)[i]->GetA()==Ar)
	     if((*IonCollection)[i]->GetZ()==Zr)
	       {
		 rBIn.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 rBIn.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 rBIn.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 rBIn.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 rBIn.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 rBIn.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 rBIn.b=(*IonCollection)[i]->GetBeta();
		 rBIn.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 rBIn.w=(*IonCollection)[i]->GetWeight();
	       }
	   break;
	 case BACKING_OUT_FLAG:
	   if((*IonCollection)[i]->GetA()==Ar)
	     if((*IonCollection)[i]->GetZ()==Zr)
	       {
		 rBOut.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 rBOut.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 rBOut.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 rBOut.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 rBOut.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 rBOut.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 rBOut.b=(*IonCollection)[i]->GetBeta();
		 rBOut.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 rBOut.w=(*IonCollection)[i]->GetWeight();
	       }
	   break;

	 default:
	   break;
	 }

    for(Int_t i=0;i<Nt;i++)
       switch((*IonCollection)[i]->GetPFlag())
	 {
	 case REACTION_IN_FLAG:
	   if((*IonCollection)[i]->GetA()==Ap)
	     if((*IonCollection)[i]->GetZ()==Zp)
	       {
		 pRIn.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 pRIn.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 pRIn.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 pRIn.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 pRIn.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 pRIn.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 pRIn.b=(*IonCollection)[i]->GetBeta();
		 pRIn.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 pRIn.w=(*IonCollection)[i]->GetWeight();
	       }
	   break;
	 case REACTION_OUT_FLAG:
	   if((*IonCollection)[i]->GetA()==Ar)
	     if((*IonCollection)[i]->GetZ()==Zr)
	       {
		 rROut.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 rROut.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 rROut.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 rROut.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 rROut.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 rROut.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 rROut.b=(*IonCollection)[i]->GetBeta();
		 rROut.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 rROut.w=(*IonCollection)[i]->GetWeight();
	       }
	   break;
	 case DECAY_FLAG:	   
	   if((*IonCollection)[i]->GetA()==Ar)
	     if((*IonCollection)[i]->GetZ()==Zr)
	       {
		 rDec.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 rDec.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 rDec.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 rDec.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 rDec.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 rDec.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 rDec.b=(*IonCollection)[i]->GetBeta();
		 rDec.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 rDec.w=(*IonCollection)[i]->GetWeight();
	       }
	   break;

	 default:
	   break;
	 }
   }       

 // G4cout<<" "<<G4endl;
 
 // G4cout<<stat.evNb<<" "<<pRIn.px<<
 //   " "<<pRIn.py<<" "<<pRIn.pz<<
 //   " "<<pRIn.E<<G4endl;
 // G4cout<<stat.evNb<<" "<<pROut.px<<
 //   " "<<pROut.py<<" "<<pROut.pz<<
 //   " "<<pROut.E<<G4endl;
 // G4cout<<stat.evNb<<" "<<rROut.px<<
 //   " "<<rROut.py<<" "<<rROut.pz<<
 //   " "<<rROut.E<<G4endl;
 // G4cout<<stat.evNb<<" "<<pRIn.px-pROut.px-rROut.px<<
 //   " "<<pRIn.py-pROut.py-rROut.py<<" "<<pRIn.pz-pROut.pz-rROut.pz<<
 //   " "<<pRIn.E-pROut.E-rROut.E<<G4endl;
 // getc(stdin);
 
 G4int Np=CsICollection->entries();
 if(Np>0)
   {
     for(Int_t i=0;i<Np;i++)
       {
	 //		       G4cout<<"*"<<(*CsICollection)[i]->GetA()<<" "<<(*CsICollection)[i]->GetZ()<<" "<<(*CsICollection)[i]->GetKE()/MeV<<" "<<(*CsICollection)[i]->GetId()<<" "<<(*CsICollection)[i]->GetPos().getX()<<" "<<(*CsICollection)[i]->GetPos().getY()<<" "<<(*CsICollection)[i]->GetPos().getZ()<<G4endl;
	 if((*CsICollection)[i]->GetA()==Ar)
	   if((*CsICollection)[i]->GetZ()==Zr)
	     {
	       if(rHit.E==0.)
		     {
		       rHit.x=(*CsICollection)[i]->GetPos().getX()/mm;
		       rHit.y=(*CsICollection)[i]->GetPos().getY()/mm;
		       rHit.z=(*CsICollection)[i]->GetPos().getZ()/mm;
		       rHit.px=(*CsICollection)[i]->GetMom().getX()/MeV;
		       rHit.py=(*CsICollection)[i]->GetMom().getY()/MeV;
		       rHit.pz=(*CsICollection)[i]->GetMom().getZ()/MeV;
		       rHit.b=(*CsICollection)[i]->GetBeta();
		       rHit.E=(*CsICollection)[i]->GetKE()/MeV;		 
		       rHit.w=(*CsICollection)[i]->GetWeight();
		       rHit.Id=(*CsICollection)[i]->GetId();
		       rHit.r=(*CsICollection)[i]->GetRingId();
		       //		       G4cout<<"+"<<(*CsICollection)[i]->GetA()<<" "<<(*CsICollection)[i]->GetZ()<<" "<<(*CsICollection)[i]->GetKE()/MeV<<" "<<(*CsICollection)[i]->GetId()<<" "<<(*CsICollection)[i]->GetPos().getX()<<" "<<(*CsICollection)[i]->GetPos().getY()<<" "<<(*CsICollection)[i]->GetPos().getZ()<<G4endl;
	     

		     }
		   else
		     {
		       //	       G4cout<<"-"<<(*CsICollection)[i]->GetA()<<" "<<(*CsICollection)[i]->GetZ()<<" "<<(*CsICollection)[i]->GetKE()/MeV<<" "<<(*CsICollection)[i]->GetId()<<" "<<(*CsICollection)[i]->GetPos().getX()<<" "<<(*CsICollection)[i]->GetPos().getY()<<" "<<(*CsICollection[i]-)[i]->GetPos().getZ()<<G4endl;
		       if((*CsICollection)[i]->GetId()==rHit.Id)
			 rHit.E+=(*CsICollection)[i]->GetKE()/MeV;	
		     }
		 }

	     if((*CsICollection)[i]->GetA()==Ap)
	       if((*CsICollection)[i]->GetZ()==Zp)
		 {
		   if(pHit.E==0.)
		     {
		       pHit.x=(*CsICollection)[i]->GetPos().getX()/mm;
		       pHit.y=(*CsICollection)[i]->GetPos().getY()/mm;
		       pHit.z=(*CsICollection)[i]->GetPos().getZ()/mm;
		       pHit.px=(*CsICollection)[i]->GetMom().getX()/MeV;
		       pHit.py=(*CsICollection)[i]->GetMom().getY()/MeV;
		       pHit.pz=(*CsICollection)[i]->GetMom().getZ()/MeV;
		       pHit.b=(*CsICollection)[i]->GetBeta();
		       pHit.E=(*CsICollection)[i]->GetKE()/MeV;		 
		       pHit.w=(*CsICollection)[i]->GetWeight();
		       pHit.Id=(*CsICollection)[i]->GetId();
		       pHit.r=(*CsICollection)[i]->GetRingId();
		     }
		   else
		     {
		       if((*CsICollection)[i]->GetId()==pHit.Id)
			 pHit.E+=(*CsICollection)[i]->GetKE()/MeV;	
		     }
		 }
	   }
	 //	 getc(stdin);
       }

     G4int i,j;
     Gfold=0;
     memset(GId,0,sizeof(GId));
     memset(GSeg,0,sizeof(GSeg));
     memset(GE,0,sizeof(GE));
     for(i=0;i<GN;i++)
       for(j=0;j<GS;j++)
	 if(gw[i][j]>0)
	   {
	     //	     G4cout<<" i "<<i<<" j "<<j<<" w "<<gw[i][j]<<G4endl;
	     GId[Gfold]=i+1;
	     GSeg[Gfold]=j;
	     Gx[Gfold]=gp[i][j].getX();
	     Gy[Gfold]=gp[i][j].getY();
	     Gz[Gfold]=gp[i][j].getZ();
	     GE[Gfold]=ge[i][j];
	     GW[Gfold]=gw[i][j];
	     Gfold++;
	   }
     //        getc(stdin);

     tree->Fill();
     IonFill++;
}
//=====================================================================================
void Results::ProjLabScattAngle()
 {
  
   Double_t theta;
   Double_t p;

   memset(&pROut,0,soi);
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();
   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();

   h=(TH1D *)gROOT->FindObject("Proj. Lab. Scatt. Angle");
   if(h==NULL)
     h=new TH1D("Proj. Lab. Scatt. Angle","Proj. Lab. Scatt. Angle",180,0,180);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("Rec. Lab. Scatt. Angle W");
   if(g==NULL)
     g=new TH1D("Proj. Lab. Scatt. Angle W","Proj. Lab. Scatt. Angle W",180,0,180);
   g->Reset();
  
   Int_t N=tree->GetEntries();
 
    for(Int_t i=0;i<N;i++)
    {
      tree->GetEntry(i);     
      p=sqrt(pROut.px*pROut.px+pROut.py*pROut.py+pROut.pz*pROut.pz);
      theta=acos(pROut.pz/p);
      h->Fill(RADIAN*theta);     
      g->Fill(RADIAN*theta,pROut.w);
    }
   h->GetXaxis()->SetTitle("theta LAB [deg.]");
   h->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   h->Draw();
   g->SetLineColor(kRed);
   g->Draw("same");
   
 }
//=====================================================================================
void Results::RecLabScattAngle()
 {
 
   Double_t theta;
   Double_t p;
  

   memset(&rROut,0,soi);
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("Rec. Lab. Scatt. Angle");
   if(h==NULL)
     h=new TH1D("Rec. Lab. Scatt. Angle","Rec. Lab. Scatt. Angle",180,0,180);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("Rec. Lab. Scatt. Angle W");
   if(g==NULL)
     g=new TH1D("Rec. Lab. Scatt. Angle W","Rec. Lab. Scatt. Angle W",180,0,180);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
    {
      tree->GetEntry(i);     
      p=sqrt(rROut.px*rROut.px+rROut.py*rROut.py+rROut.pz*rROut.pz);
      theta=acos(rROut.pz/p);
      h->Fill(RADIAN*theta);     
      g->Fill(RADIAN*theta,rROut.w);
    }
   h->GetXaxis()->SetTitle("theta LAB [deg.]");
   h->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   h->Draw();
   g->SetLineColor(kRed);
   g->Draw("same");
 }
//=====================================================================================
void Results::CrossSectionZProfile()
 {
 
   Double_t zmin=1000.,zmax=-1000.;
   Double_t s;

   memset(&rROut,0,soi);
   memset(&pRIn,0,soi);
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   Int_t N=tree->GetEntries();
   for(Int_t i=0;i<N;i++)
    {
      tree->GetEntry(i); 
      if(pRIn.z==0) printf("%d\n",i);
      if(pRIn.z<zmin) zmin=pRIn.z;
      if(pRIn.z>zmax) zmax=pRIn.z;
    }
 
   s=zmax-zmin;
   s*=0.1;
   zmin-=s;
   zmax+=s;
   
   h=(TH1D *)gROOT->FindObject("Cross Section vs. Z");
   if(h==NULL)
     h=new TH1D("Cross Section vs. Z","Cross Section vs. Z",100,zmin,zmax);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("Cross Section vs. Z W");
   if(g==NULL)
     g=new TH1D("Cross Section vs. Z W","Cross Section vs. Z W",100,zmin,zmax);
   g->Reset();

   
  
   for(Int_t i=0;i<N;i++)
    {
      tree->GetEntry(i);     
      h->Fill(rROut.z);     
      g->Fill(rROut.z,rROut.w);
    }

   g->Divide(h);
   h->Divide(h);
   g->Scale(TargetFaceCrossSection);
   h->Scale(TargetFaceCrossSection);
   h->GetXaxis()->SetTitle("Z");
   h->GetYaxis()->SetTitle("Cross section [b]");
   h->SetLineColor(kBlue);
   h->Draw();
   g->SetLineColor(kRed);
   g->Draw("same");
 }
//=====================================================================================
void Results::TargetFaceF_CM()
 {
  Double_t theta;
  Double_t px,py,pz,p,f,dp;
  
   memset(&stat,0,sos);
   memset(&pRIn,0,soi);
   memset(&pROut,0,soi);
   memset(&rROut,0,soi);
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("CM Scatt. Angle");
   if(h==NULL)
     h=new TH1D("CM Scatt. Angle","CM Scatt. Angle",180,0,180);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("CM Scatt. Angle W");
   if(g==NULL)
     g=new TH1D("CM Scatt. Angle W","CM Scatt. Angle W",180,0,180);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
    {
      tree->GetEntry(i);
      f=(Double_t)stat.Ap/((Double_t)(stat.Ap+stat.Ar));
      px=pROut.px-f*pRIn.px;
      py=pROut.py-f*pRIn.py;
      pz=pROut.pz-f*pRIn.pz;
      p=sqrt(px*px+py*py+pz*pz);
      dp=px*pRIn.px+py*pRIn.py+pz*pRIn.pz;
      f=sqrt(pRIn.px*pRIn.px+pRIn.py*pRIn.py+pRIn.pz*pRIn.pz);
      theta=RADIAN*acos(dp/p/f);
      h->Fill(theta);     
      g->Fill(theta,rROut.w);
    }
   g->GetXaxis()->SetTitle("theta CM [deg.]");
   g->GetYaxis()->SetTitle("Counts");
   //  h->SetLineColor(kBlue);
   // h->Draw();
   g->SetLineColor(kRed);
   g->Draw();

   vector <G4double> *th=new vector <G4double>;
   vector <G4double> *ff=new vector <G4double>;
   th=theProjectile->getth();
   ff=theProjectile->getf();
   
   vector<G4double>::iterator it=th->begin();

  
   Int_t n=0,i;
   for(;it<th->end();it++) n++;
   Double_t x[20],y[20];
   if(n>20) n=20;
   Double_t tn=theProjectile->getthef();
   Double_t norm=0.5*tn*RADIAN/PI;
   norm=N/norm;
   for(i=0;i<n;i++)
     {
       x[i]=(*th)[i];
       y[i]=norm*(*ff)[i]*sin((*th)[i]/RADIAN);
     }
   TGraph *gr1 = new TGraph (n, x, y);
   gr1->Draw("same");

 }
//=====================================================================================
G4int Results::RingMap(G4int id,G4int seg)
 {
   G4int r,s;
   s=-1;
   if(seg==0) s=0;
   if(seg==1) s=1;
   if(seg==2) s=1;
   if(seg==3) s=0;
   if(s<0) return -1;

   r=-1;
   switch (id)
     {
     case  1:r=1;break;
     case  2:r=1;break;
     case  3:r=1;break;
     case  4:r=1;break;
     case  5:r=3;break;
     case  6:r=3;break;
     case  7:r=3;break;
     case  8:r=3;break;
     case  9:r=3;break;
     case 10:r=3;break;
     case 11:r=3;break;
     case 12:r=3;break;
     case 13:r=5;break;
     case 14:r=5;break;
     case 15:r=5;break;
     case 16:r=5;break;
     default:break;
     }
   if(r<0) return -1;

   return r+s;
 }
//=====================================================================================
void Results::CsIRingSpectrum(G4int ring)
 {
 
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("CsI");
   if(h==NULL)
     h=new TH1D("CsI","CsI",256,0,256);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("CsI W");
   if(g==NULL)
     g=new TH1D("CsI W","CsI W",256,0,256);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       if(rHit.r==ring)
	   {
	     h->Fill(rHit.E);     
	     g->Fill(rHit.E,rHit.w);
	   }
       // if(pHit.r==ring)
       // 	   {
       // 	     h->Fill(pHit.E);     
       // 	     g->Fill(pHit.E,pHit.w);
       // 	   }
     }
	   
   g->GetXaxis()->SetTitle("Energy [MeV]");
   g->GetYaxis()->SetTitle("Counts");
   // h->SetLineColor(kBlue);
   // h->Draw();
   g->SetLineColor(kRed);
   g->Draw();

 }
//=====================================================================================
void Results::GammaRingSpectrum(G4int ring)
 {
 
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("Gamma");
   if(h==NULL)
     h=new TH1D("Gamma","Gamma",4096,0,4096);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("Gamma W");
   if(g==NULL)
     g=new TH1D("Gamma W","Gamma W",4096,0,4096);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       for(Int_t j=0;j<Gfold;j++)
	 if(RingMap(GId[j],GSeg[j])==ring)
	   {
	     h->Fill(FWHM_response(GE[j]));     
	     g->Fill(FWHM_response(GE[j]),GW[j]);
	   }
     }
	   
   g->GetXaxis()->SetTitle("Energy [keV]");
   g->GetYaxis()->SetTitle("Counts");
   //  h->SetLineColor(kBlue);
   // h->Draw();
   g->SetLineColor(kRed);
   g->Draw();

 }
 //=====================================================================================
void Results::DetCryCsIGammaSpectrum(G4int det,G4int cry, G4int pin)
 {
   G4String name="Det. ";
   
   name.append((G4String)det);
   name.append(" Cry. ");
   name.append((G4String)cry);
   name.append(" CsI ");
   name.append((G4String)pin);

   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("Gamma");
   if(h==NULL)
     h=new TH1D("Gamma","Gamma",4096,0,4096);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("Gamma W");
   if(g==NULL)
     g=new TH1D("Gamma W","Gamma W",4096,0,4096);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       if(rHit.Id==pin)
	 for(Int_t j=0;j<Gfold;j++)
	   if(GId[j]==det)
	     if(GSeg[j]==cry)
	       {
		 h->Fill(FWHM_response(GE[j]));     
		 g->Fill(FWHM_response(GE[j]),GW[j]);
	       }
     }
	   
   h->GetXaxis()->SetTitle("Energy [keV]");
   h->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   h->SetTitle(name);
   h->Draw();
   g->SetLineColor(kRed);
   g->SetTitle(name);
   g->Draw("same");

 }
  //=====================================================================================
void Results::DetRingCsIRingGammaSpectrum(G4int detRing,G4int pinRing,G4double coeff)
 {
   char name[132];
   G4double eg,ec;

   sprintf(name,"Gamma Ring %1d CsI Ring %1d",detRing,pinRing);
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv1","StdCanv1",700,500);
   c->Clear();
   c->Divide(1,2);

 

 
   correlation=(TH2D *)gROOT->FindObject("ECsI_EGamma");
   if(correlation==NULL)
     correlation=new TH2D("ECsI_EGamma","ECsI_EGamma",256,0,256,256,0,1024);
   correlation->Reset();

   h=(TH1D *)gROOT->FindObject("Gamma");
   if(h==NULL)
     h=new TH1D("Gamma","Gamma",256,0,1024);
   h->Reset();

  
   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       if(rHit.r==pinRing)
	 for(Int_t j=0;j<Gfold;j++)
	   if(RingMap(GId[j],GSeg[j])==detRing)
	       {
		 ec=rHit.E/MeV;
		 eg=FWHM_response(GE[j])-coeff*ec;
		 correlation->Fill(ec,eg,GW[j]);
		 h->Fill(eg,GW[j]);
	       }
     }

   c->cd(1);
   gStyle->SetPalette(1);
   correlation->GetXaxis()->SetTitle("ECsI [MeV]");
   correlation->GetYaxis()->SetTitle("EGamma [keV]");
   correlation->SetTitle(name);
   correlation->Draw("COLZ");
   c->cd(2);
   h->GetXaxis()->SetTitle("Uncorrelated Energy [arb.]");
   h->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   h->SetTitle(name);
   h->Draw();  
 }
 
//=====================================================================================
void Results::CalculateCrystalPositions()
 {
   Double_t x[GN][GS],y[GN][GS],z[GN][GS],n[GN][GS];
   Double_t xx,yy,zz;
   size_t sod=GN*4*sizeof(Double_t);
   memset(x,0,sod);
   memset(y,0,sod);
   memset(z,0,sod);
   memset(n,0,sod);
   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       for(Int_t j=0;j<Gfold;j++)
	 {
	   x[GId[j]-1][GSeg[j]]+=Gx[j];
	   y[GId[j]-1][GSeg[j]]+=Gy[j];
	   z[GId[j]-1][GSeg[j]]+=Gz[j];
	   n[GId[j]-1][GSeg[j]]++;
	 }
     }

   for(Int_t det=0;det<GN;det++)
     for(Int_t cry=0;cry<GS;cry++)
       {
	 xx=x[det][cry]/n[det][cry];
	 yy=y[det][cry]/n[det][cry];
	 zz=z[det][cry]/n[det][cry];
	 CP[det][cry].setX(xx);
	 CP[det][cry].setY(yy);
	 CP[det][cry].setZ(zz);
       }
   ReportCrystalPositions();
 }
//=====================================================================================
void Results::ReportCrystalPositions()
 {
   Double_t xx,yy,zz,rr,cc;
   printf("det cry ring     x        y        z        r       cos\n");
   for(Int_t det=0;det<GN;det++)
     for(Int_t cry=0;cry<GS;cry++)
       {
	 xx=CP[det][cry].getX();
	 yy=CP[det][cry].getY();
	 zz=CP[det][cry].getZ();
	 rr=sqrt(xx*xx+yy*yy+zz*zz);
	 cc=zz/rr;
	 printf(" %2d  %2d  %3d %8.3f %8.3f %8.3f %8.3f %8.3f\n",det+1,cry,RingMap(det+1,cry),xx,yy,zz,rr,cc);
       }
       
 } 
//=====================================================================================
void Results::CalculateCsIPositions()
 {
   Double_t x[NCsI],y[NCsI],z[NCsI],n[NCsI];
   Double_t xx,yy,zz;
   size_t sod=NCsI*sizeof(Double_t);
   memset(x,0,sod);
   memset(y,0,sod);
   memset(z,0,sod);
   memset(n,0,sod);
   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       if(pHit.E>0)
	 {
	   x[(Int_t)pHit.Id-1]+=pHit.x;
	   y[(Int_t)pHit.Id-1]+=pHit.y;
	   z[(Int_t)pHit.Id-1]+=pHit.z;
	   n[(Int_t)pHit.Id-1]++;
	 }
       if(rHit.E>0)
	 {
	   x[(Int_t)rHit.Id-1]+=rHit.x;
	   y[(Int_t)rHit.Id-1]+=rHit.y;
	   z[(Int_t)rHit.Id-1]+=rHit.z;
	   n[(Int_t)rHit.Id-1]++;
	 }
     }

   for(Int_t det=0;det<NCsI;det++)
       {
	 xx=x[det]/n[det];
	 yy=y[det]/n[det];
	 zz=z[det]/n[det];
	 PP[det].setX(xx);
	 PP[det].setY(yy);
	 PP[det].setZ(zz);
       }
   ReportCsIPositions();
 }
//=====================================================================================
void Results::ReportCsIPositions()
 {
   Double_t xx,yy,zz,rr,cc;
   printf("pin      x        y        z        r       cos \n");
   for(Int_t det=0;det<NCsI;det++)
       {
	 xx=PP[det].getX();
	 yy=PP[det].getY();
	 zz=PP[det].getZ();
	 rr=sqrt(xx*xx+yy*yy+zz*zz);
	 cc=zz/rr;
	 printf(" %2d  %8.3f %8.3f %8.3f %8.3f %8.3f\n",det+1,xx,yy,zz,rr,cc);
       }
       
 } 
//=====================================================================================
void Results::GroupCosDist()
 {
 
   Int_t pin,det,cry;
   Double_t cos;

   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("GroupCos");
   if(h==NULL)
     h=new TH1D("GroupCos","GroupCos",1000,0.0,1.0);
   h->Reset();
 
   for(pin=0;pin<NCsI;pin++)
     for(det=0;det<GN;det++)
       for(cry=0;cry<GS;cry++)
	 {
	   cos=PP[pin].dot(CP[det][cry])/PP[pin].mag()/CP[det][cry].mag();
	   h->Fill(cos);
	 }
  
   h->GetXaxis()->SetTitle("cos(theta)");
   h->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kRed);
   h->Draw();

 }
//=====================================================================================
G4double Results::FWHM_response(G4double e_in)
{
  G4double e_out,fwhm,sigma,e,channel;
  
  e=e_in/1000.;
  // printf("e %f ein %f\n",e,e_in);
  fwhm=sqrt(F+G*e+H*e*e);
  channel=A+B*e_in+C*e_in*e_in;
  sigma=fwhm/2.35482;
  //  printf("channel %f sigma %f\n",channel,sigma);
  if(sigma>0)
    e_out=CLHEP::RandGauss::shoot(channel,sigma);
  else
    e_out=channel;

  //  printf("e_out  %f\n",e_out);
  //  getc(stdin);
  return e_out;
}

//=====================================================================================
G4double Results::CalculatePath(G4ThreeVector iPos, G4ThreeVector Pos)
{
  G4double xd,yd,zd;
  G4double dist;

  xd=Pos.getX()-iPos.getX();
  yd=Pos.getY()-iPos.getY();
  zd=Pos.getZ()-iPos.getZ();

  dist=sqrt(xd*xd+yd*yd+zd*zd);

  return dist;
}