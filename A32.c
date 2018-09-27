#include "DataflowAnalysis.h"



namespace dataflow{

    struct LivenessAnalysis: public DataflowAnalysis{

        static char ID;

        LivenessAnalysis() : DataflowAnalysis(ID){}

        

    protected:

        /**

         *  Implement your analysis in this function. Store your results in DataflowAnalysis::inMap and

         *  DataflowAnalysis:outMap.

         */

        void doAnalysis(Function &F) override{
		SetVector<Value*>* instructions = new SetVector<Value*>;
		bool cont = false;	

                bool cont2 = false;	

		while (cont == false){  

			cont = true;

			for(inst_iterator I = (--inst_end(F)), E= inst_begin(F); I != E; I--){
			   // errs() << "**Instruction: " << *I << "\n";
				cont2 = false;

				SetVector<Value*>* outVec = outMap[&(*I)];

				SetVector<Value*>* inVec = inMap[&(*I)] ;

				for(Instruction* S : getSuccessors(&(*I))){
					SetVector<Value*>* inMapofSucc = inMap[&(*S)];
					if(inMapofSucc->size() > 0){

			                         for(SetVector<Value*>::iterator V = inMapofSucc->begin(), VE = inMapofSucc->end(); V != VE; ++V){
						
							cont2 = false;

							for(SetVector<Value*>::iterator IN = outVec->begin(), INE = outVec->end(); IN != INE; ++IN){

								if(*V == *IN){

									cont2=true;

									

								}

							}	

							if(cont2 == false){

								inVec->insert(*V);

							    	outVec->insert(*V);

								cont = false;		

							}



						}

				        }
				}
				cont2 = false;
				for (Use& U : I->operands()){

					Value* v = U.get();

					// Check if Value v is a local variable.

					if(isa<Instruction>(v)){
						
						for(SetVector<Value*>::iterator IN = inVec->begin(), INE = inVec->end(); IN != INE; ++IN){

							if(&(*v) == *IN){

								cont2=true;

							}

						}	

						if(cont2 == false){

							//inVec->insert(*V);

						    	inVec->insert(&(*v));

							cont=false;	

						}

						


						//inVec->insert(&(*v));
					}

						//errs() << *v << ", ";

				}
				cont2 = false;
				if (isDef(&(*I))){

					

			                for(SetVector<Value*>::iterator IN = inVec->begin(), INE = inVec->end(); IN != INE; ++IN){

						if(&(*I) == *IN){

							cont2=true;
							inVec->remove(&(*I));

							cont=false;

						}

					}	

					

			                //outMap[&(*I)] = outVec;

					//inVec->remove(&(*I));

					//outVec->insert(&(*I));

				//cont=false;

				}
			
			

			
			}
		}
		for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I){
			SetVector<Value*>* outVec = outMap[&(*I)];
			SetVector<Value*>* inVec = inMap[&(*I)] ;

			for(Instruction* S : getSuccessors(&(*I))){
				SetVector<Value*>* inMapofSucc = inMap[&(*S)];
				if(inMapofSucc->size() > 0){

		                         for(SetVector<Value*>::iterator V = inMapofSucc->begin(), VE = inMapofSucc->end(); V != VE; ++V){
						inVec->insert(*V);

						outVec->insert(*V);

					}

			        }
			}
			for (Use& U : I->operands()){

				Value* v = U.get();

				// Check if Value v is a local variable.
				if(isa<Instruction>(v)){	
					inVec->insert(&(*v));	//inVec->insert(&(*v));
				}
			}
			if (isDef(&(*I))){	

				inVec->remove(&(*I));

			}
			break;
		}

        }

        

        virtual std::string getAnalysisName() override{

            return "Liveness Analysis";

        }

    };

    

    char LivenessAnalysis::ID = 1;

    static RegisterPass<LivenessAnalysis> X("Liveness", "Liveness Analysis",

                                            false /* Only looks at CFG */,

                                            false /* Analysis Pass */);

}