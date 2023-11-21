#ifdef NEW_PROTOCOL
// parse commands
void parse_engine( uint8_t * rxmsg, uint32_t len)
{
	uint8_t is_ready_to_parse = recv_buffer_processing(rxmsg, len);
	if (!is_ready_to_parse) return;
	
	rxmsg = (uint8_t*) cmd_buffer;
	uint32_t i;

// $reset			
					if( (strstr((char *) rxmsg ,"$") != NULL) )
					{
							net_SendString( (uint8_t *) str1, strlen((const char *)str1));
							NVIC_SystemReset();
							return;
					}

// reset			
					if( (strstr((char *) rxmsg ,"RESET") != NULL) && (strlen((char *) rxmsg) == 5))
					{
							net_SendString( (uint8_t *) str1r, strlen((const char *)str1r));
							Reset_System();
							return;
					}
		
					
// MOTOR1		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "MOTOR1=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												err = motor1_on();
												if(err == 0)
													net_SendString((uint8_t *) str2, strlen((const char *)str2));
												else
													net_SendString((uint8_t *) str4, strlen((const char *)str4));
										}else
										{
												motor1_off();
												net_SendString((uint8_t *) str3, strlen((const char *)str3));
										}
							}
							else
							{
										net_SendString((uint8_t *) str4, strlen((const char *)str4));
							}
							return;
					}

// MOTOR2		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "MOTOR2=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												err = motor2_on(0);
												if(err == 0)
													net_SendString((uint8_t *) str5, strlen((const char *)str5));
												else
													net_SendString((uint8_t *) str7, strlen((const char *)str7));
										}
										else
										{
												motor2_off();
												net_SendString((uint8_t *) str6, strlen((const char *)str6));
										}
							}
							else
							{
										net_SendString((uint8_t *) str7, strlen((const char *)str7));
							}
							return;
					}
					
// MOTOR3		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "MOTOR3=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
											{
												motor3_on();
												net_SendString((uint8_t *) str8, strlen((const char *)str8));
										}else
										{
												motor3_off();
												net_SendString((uint8_t *) str9, strlen((const char *)str9));
										}
							}
							else
							{
										net_SendString((uint8_t *) str10, strlen((const char *)str10));
							}
							return;
					}
// MOTOR4		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "MOTOR4=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
											{
												motor4_on();
												net_SendString((uint8_t *) str11, strlen((const char *)str11));
										}else
										{
												motor4_off();
												net_SendString((uint8_t *) str12, strlen((const char *)str12));
										}
							}
							else
							{
										net_SendString((uint8_t *) str13, strlen((const char *)str13));
							}
							return;
					}

					
// set relay				
					if( strstr((char *) rxmsg, "RELAY") != NULL )
					{
							ptrx =  strstr((char *) rxmsg, "Y" );
							tmp[0] = *( ptrx + 1);
							tmp[1] = 0x00;
							arg1 = atoi( tmp );
							if( arg1 > 0 && arg1 < 9)
							{
								ptrx =  strstr((char *) rxmsg, "=" );
								tmp[0] = *( ptrx + 1);
								tmp[1] = 0x00;
								arg2 = atoi( tmp );
								relay( arg1, arg2);
								net_SendString((uint8_t *) str14, strlen((const char *)str14));
						}
						else
						{
									net_SendString((uint8_t *) str15, strlen((const char *)str15));
						}
						return;
					}

// BREAK_N=x				
					if( strstr((char *) rxmsg, "BREAK_N=") != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							tmp[0] = *( ptrx + 1);
							tmp[1] = 0x00;
							arg1 = atoi( tmp );
							if( arg1 == 1)
							{
								err = motor1_on();
								if(err == 0)
									net_SendString((uint8_t *) str16, strlen((const char *)str16));
								else
									net_SendString((uint8_t *) str17, strlen((const char *)str17));
								err = motor2_on(0);
								if(err == 0)
								{
									net_SendString((uint8_t *) str18, strlen((const char *)str18));
									net_SendString((uint8_t *) str20, strlen((const char *)str20));
									return;
								}
								else
									net_SendString((uint8_t *) str19, strlen((const char *)str19));
								return;
							} else 
								if( arg1 == 0)
								{
									motor1_off();
									motor2_off();
									net_SendString((uint8_t *) str21, strlen((const char *)str21));
									return;
								}
					}

// BREAK_I=x				
					if( strstr((char *) rxmsg, "BREAK_I=") != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							tmp[0] = *( ptrx + 1);
							tmp[1] = 0x00;
							arg1 = atoi( tmp );
							if( arg1 == 1)
							{
								err = motor1_on();
								if(err == 0)
									net_SendString((uint8_t *) str160, strlen((const char *)str160));
								else
								{
									net_SendString((uint8_t *) str170, strlen((const char *)str170));
									
								}
								err = motor2_on(1);
								if(err == 0)
								{
									net_SendString((uint8_t *) str180, strlen((const char *)str180));
									net_SendString((uint8_t *) str200, strlen((const char *)str200));
								}
								else
									net_SendString((uint8_t *) str190, strlen((const char *)str190));
								return;
							} else 
								if( arg1 == 0)
								{
									motor1_off();
									motor2_off();
									net_SendString((uint8_t *) str210, strlen((const char *)str210));
									return;
								}
					}

// BREAK_OFF				
					if( strstr((char *) rxmsg, "BREAK=OFF") != NULL)
					{
								motor1_off();
								motor2_off();
								net_SendString((uint8_t *) str220, strlen((const char *)str220));
								return;
					}


// BREAK_L=x				
					if( strstr((char *) rxmsg, "BREAK_L=") != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							tmp[0] = *( ptrx + 1);
							tmp[1] = 0x00;
							arg1 = atoi( tmp );
							if( arg1 == 1)
							{
								err = motor1_on();
								if(err == 0)
									net_SendString((uint8_t *) str22, strlen((const char *)str22));
								else
								{
									net_SendString((uint8_t *) str23, strlen((const char *)str23));
								}
								return;
							} else 
								if( arg1 == 0)
								{
									motor1_off();
									net_SendString((uint8_t *) str24, strlen((const char *)str24));
									return;
								}
					}

// BREAK_R=x				
					if( strstr((char *) rxmsg, "BREAK_R=") != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							tmp[0] = *( ptrx + 1);
							tmp[1] = 0x00;
							arg1 = atoi( tmp );
							if( arg1 == 1)
							{
								err = motor2_on(1);
								if(err == 0)
									net_SendString((uint8_t *) str25, strlen((const char *)str25));
								else
								{
									net_SendString((uint8_t *) str26, strlen((const char *)str26));
								}
								return;
							} else 
								if( arg1 == 0)
								{
									motor1_off();
									net_SendString((uint8_t *) str27, strlen((const char *)str27));
									return;
								}
					}

// BREAK_EXN=x				
					if( strstr((char *) rxmsg, "BREAK_EXN=") != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							tmp[0] = *( ptrx + 1);
							tmp[1] = 0x00;
							arg1 = atoi( tmp );
							if( arg1 == 1)
							{
								err = motorx1_on();
								if(err == 0)
									net_SendString((uint8_t *) str28, strlen((const char *)str28));
								else
									net_SendString((uint8_t *) str29, strlen((const char *)str29));
								err = motorx2_on(0);
								if(err == 0)
									net_SendString((uint8_t *) str30, strlen((const char *)str30));
								else
									net_SendString((uint8_t *) str31, strlen((const char *)str31));
								return;
							} else 
								if( arg1 == 0)
								{
									motor1_off();
									motor2_off();
									net_SendString((uint8_t *) str32, strlen((const char *)str32));
									return;
								}
					}

// BREAK_EXI=x				
					if( strstr((char *) rxmsg, "BREAK_EXI=") != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							tmp[0] = *( ptrx + 1);
							tmp[1] = 0x00;
							arg1 = atoi( tmp );
							if( arg1 == 1)
							{
								err = motorx1_on();
								if(err == 0)
									net_SendString((uint8_t *) str33, strlen((const char *)str33));
								else
								{
									net_SendString((uint8_t *) str34, strlen((const char *)str34));
									
								}
								err = motorx2_on(1);
								if(err == 0)
									net_SendString((uint8_t *) str35, strlen((const char *)str35));
								else
									net_SendString((uint8_t *) str36, strlen((const char *)str36));
								return;
							} else 
								if( arg1 == 0)
								{
									motor1_off();
									motor2_off();
									net_SendString((uint8_t *) str37, strlen((const char *)str37));
									return;
								}
					}

					

// SUSP CYCLE Left				

					if( strstr((char *) rxmsg, "SUSP_CYCLE_L") != NULL)
					{
							net_SendString((uint8_t *) str38, strlen((const char *)str38));
							net_SendString((uint8_t *) str39, strlen((const char *)str39));
							motor3_on();
							HAL_Delay( susp_delay );
							motor3_off();
							net_SendString((uint8_t *) str40, strlen((const char *)str40));
							susp_cnt_samples = susp_cnt_samples;
							net_SendString((uint8_t *) str41, strlen((const char *)str41));
							memcpy( suspmsg, str42, 20);
						return;
					}
					

// SUSP CYCLE Right				

					if( strstr((char *) rxmsg, "SUSP_CYCLE_R") != NULL)
					{
							net_SendString((uint8_t *) str43, strlen((const char *)str43));
							net_SendString((uint8_t *) str44, strlen((const char *)str44));
							motor4_on();
							HAL_Delay( susp_delay );
							motor4_off();
							net_SendString((uint8_t *) str45, strlen((const char *)str45));
							susp_cnt_samples = susp_cnt_samples;
							net_SendString((uint8_t *) str46, strlen((const char *)str46));
							memcpy( suspmsg, str47, 20);
						return;
					}
					
// SUSP_TIME = 		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SUSP_TIME=" ); 
					if(ptrx != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if(isdigit(strini[i])==0) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( (arg2 < 500) && (arg2  > 10000)) err = 1;
							if( err == 0)
							{
									susp_delay = arg2;
									net_SendString((uint8_t *) str48, strlen((const char *)str48));
						}
						else
						{
									net_SendString((uint8_t *) str49, strlen((const char *)str49));
						}
						return;
					}
					
// SUSP_SAMPLES = 		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SUSP_SAMPLES=" ); 
					if(ptrx != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if(isdigit(strini[i])==0) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( (arg2 < 50) && (arg2  > 1000)) err = 1;
							if( err == 0)
							{
									susp_samples = arg2;
									net_SendString((uint8_t *) str50, strlen((const char *)str50));
						}
						else
						{
									net_SendString((uint8_t *) str51, strlen((const char *)str51));
						}
						return;
					}
// PACK_TIME = 		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "PACK_TIME=" ); 
					if(ptrx != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if(isdigit(strini[i])==0) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( (arg2 < 15) && (arg2  > 1000)) err = 1;
							if( err == 0)
							{
									pack_time = arg2;
									net_SendString((uint8_t *) str52, strlen((const char *)str52));
						}
						else
						{
									net_SendString((uint8_t *) str53, strlen((const char *)str53));
						}
						return;
					}

// SEND_PACK=x		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SEND_PACK=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												kpacket = 1;
												net_SendString((uint8_t *) str54, strlen((const char *)str54));
										}else
										{
												kpacket = 0;
												net_SendString((uint8_t *) str55, strlen((const char *)str55));
										}
							}
							else
							{
										net_SendString((uint8_t *) str56, strlen((const char *)str56));
							}
							return;
					}
					
					
// DIAG=x		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "DIAG=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												kdiag = 1;
												net_SendString((uint8_t *) str57, strlen((const char *)str57));
										}else
										{
												kdiag = 0;
												net_SendString((uint8_t *) str58, strlen((const char *)str58));
										}
							}
							else
							{
										net_SendString((uint8_t *) str59, strlen((const char *)str59));
							}
							return;
					}
					
// ZEROS=x		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "ZEROS=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( (tmp[0] != '0') && (tmp[0] != '1') && (tmp[0] != '?') ) err = 1;
							if( err == 0 )
							{
										if(tmp[0] == '?') 
										{
											memset( prtmsg, 0x00, sizeof( prtmsg ));
											sprintf( prtmsg, "@Zeros=OK:\r\n");
											for(x=0; x<6; x++)
											{
												memset( tmpmsg, 0x00, 64);
												sprintf( tmpmsg, "@Zero[%d]=%04u\r\n", x, zero[x]);
												strcat( prtmsg, tmpmsg);
											}
											net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
											return;
										}
										else
										{
											arg1 = (uint32_t) atoi(tmp);
											if( arg1 == 1) 
											{
													kdiag = 1;
													net_SendString((uint8_t *) str60, strlen((const char *)str60));
											}
											else
											{
													kdiag = 0;
													net_SendString((uint8_t *) str61, strlen((const char *)str61));
											}
										}
							}
							else
							{
										net_SendString((uint8_t *) str62, strlen((const char *)str62));
							}
							return;
					}

// LOGIC_NEG=x		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "LOGIC_NEG=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( (tmp[0] != '0') && (tmp[0] != '1') && (tmp[0] != '?') ) err = 1;
							if( err == 0 )
							{
										if(tmp[0] == '?') 
										{
											memset( prtmsg, 0x00, sizeof( prtmsg ));
											sprintf( prtmsg, "@LOGIC_NEG=%d\r\n", klogicneg );
											net_SendString((uint8_t *) prtmsg, strlen((const char *)prtmsg));
											return;
										}
										else
										{
											arg1 = (uint32_t) atoi(tmp);
											if( arg1 == 1) 
											{
													kdiag = 1;
													net_SendString((uint8_t *) str63, strlen((const char *)str63));
											}
											else
											{
													kdiag = 0;
													net_SendString((uint8_t *) str64, strlen((const char *)str64));
											}
										}
							}
							else
							{
										net_SendString((uint8_t *) str65, strlen((const char *)str65));
							}
							return;
					}

					
// SPOSL=x		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SPOSL=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												ksposl = 1;
												net_SendString((uint8_t *) str66, strlen((const char *)str66));
										}else
										{
												ksposl = 0;
												net_SendString((uint8_t *) str66, strlen((const char *)str66));
										}
							}
							else
							{
										net_SendString((uint8_t *) str67, strlen((const char *)str67));
							}
							return;
					}
					
// SPOSR=x		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SPOSR=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												ksposr = 1;
												net_SendString((uint8_t *) str68, strlen((const char *)str68));
										}else
										{
												ksposr = 0;
												net_SendString((uint8_t *) str69, strlen((const char *)str69));
										}
							}
							else
							{
										net_SendString((uint8_t *) str70, strlen((const char *)str70));
							}
							return;
					}
					
// SSPDL=x		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SSPDL=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												ksspdl = 1;
												net_SendString((uint8_t *) str71, strlen((const char *)str72));
										}else
										{
												ksspdl = 0;
												net_SendString((uint8_t *) str73, strlen((const char *)str73));
										}
							}
							else
							{
										net_SendString((uint8_t *) str74, strlen((const char *)str74));
							}
							return;
					}
					
// SSPDR=x		
					ptrx = NULL;
					ptrx = strstr((char *) rxmsg, "SSPDR=" ); 
					if(ptrx != NULL)
					{
						  err = 0;
							ptrx = strstr( (char *) rxmsg, "=");
							tmp[0] = *(ptrx + 1);
							tmp[1] = 0x00;
						  if( ! isdigit( tmp[0] )) 
								err = 1;
							else
								arg1 = (uint32_t) atoi(tmp);
							if( err == 0 )
							{
										if( arg1 == 1) 
										{
												ksspdr = 1;
												net_SendString((uint8_t *) str75, strlen((const char *)str75));
										}else
										{
												ksspdr = 0;
												net_SendString((uint8_t *) str76, strlen((const char *)str76));
										}
							}
							else
							{
										net_SendString((uint8_t *) str77, strlen((const char *)str77));
							}
							return;
					}
					
// MSAVE				

					if( strstr((char *) rxmsg, "MSAVE") != NULL)
					{
							if( memory_save() == 0)
							{
								net_SendString((uint8_t *) str78, strlen((const char *)str78));
							}
							else
							{
								net_SendString((uint8_t *) str37, strlen((const char *)str37));
							}
							return;
					}		

					
// set-hash-led 1				

					if( strstr((char *) rxmsg, "set-hash-led 1") != NULL)
					{
							HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
							net_SendString((uint8_t *) str36, strlen((const char *)str36));
							return;
					}
					else if ( strstr((char *) rxmsg, "set-hash-led ") != NULL) {
							net_SendString((uint8_t *) str37, strlen((const char *)str37));
							return;
					}
		
// get-version			
					if( (strstr((char *) rxmsg ,"GET-VERSION") != NULL) && (strlen((char *) rxmsg) < 12))
					{
							net_SendString((uint8_t *) strVersion, strlen((const char *)strVersion));
							return;
					}

// set keep alive
				// sintaxe : set-keepalive xxxx\n
				// xxxx -> miliseconds 0 to 99999
					if( strstr((char *) rxmsg, "SET_KEEPALIVE=") != NULL)
					{
							ptrx =  strstr((char *) rxmsg, "=" );
							strini = ( ptrx + 1);
							ptrx = strchr((char *) rxmsg, 0x00 );
							*( ptrx ) = 0x00;
							err = 0;
							for( i = 0; i < strlen(strini)-1; i++)
							{
									if(isdigit(strini[i])==0) err = 1;
							}
							arg2 = (uint32_t) atoi( strini );
							if( (arg2 > 0 && arg2  < 100) || (arg2 > 120000)) err = 1;
							if( err == 0)
							{
									keepaliveperiod = arg2;
									net_SendString((uint8_t *) str100, strlen((const char *)str100));
						}
						else
						{
									net_SendString((uint8_t *) str110, strlen((const char *)str110));
						}
						return;
					}
// GET_CONFIG

				if( (strstr((char *) rxmsg, "GET_CONFIG") != NULL) && (strlen((const char *)rxmsg) == 10))
				{	
					// show configs	
					return;
				}

// invalid command
				net_SendString((uint8_t *) str250, strlen((const char *)str250));
}
#else
