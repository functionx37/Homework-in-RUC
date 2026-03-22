
bomb:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 d9 4f 00 00 	mov    0x4fd9(%rip),%rax        # 5fe8 <__gmon_start__>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	call   *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	ret

Disassembly of section .plt:

0000000000001020 <getenv@plt-0x10>:
    1020:	ff 35 e2 4f 00 00    	push   0x4fe2(%rip)        # 6008 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	ff 25 e4 4f 00 00    	jmp    *0x4fe4(%rip)        # 6010 <_GLOBAL_OFFSET_TABLE_+0x10>
    102c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000001030 <getenv@plt>:
    1030:	ff 25 e2 4f 00 00    	jmp    *0x4fe2(%rip)        # 6018 <getenv@GLIBC_2.2.5>
    1036:	68 00 00 00 00       	push   $0x0
    103b:	e9 e0 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001040 <strcasecmp@plt>:
    1040:	ff 25 da 4f 00 00    	jmp    *0x4fda(%rip)        # 6020 <strcasecmp@GLIBC_2.2.5>
    1046:	68 01 00 00 00       	push   $0x1
    104b:	e9 d0 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001050 <__errno_location@plt>:
    1050:	ff 25 d2 4f 00 00    	jmp    *0x4fd2(%rip)        # 6028 <__errno_location@GLIBC_2.2.5>
    1056:	68 02 00 00 00       	push   $0x2
    105b:	e9 c0 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001060 <strcpy@plt>:
    1060:	ff 25 ca 4f 00 00    	jmp    *0x4fca(%rip)        # 6030 <strcpy@GLIBC_2.2.5>
    1066:	68 03 00 00 00       	push   $0x3
    106b:	e9 b0 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001070 <puts@plt>:
    1070:	ff 25 c2 4f 00 00    	jmp    *0x4fc2(%rip)        # 6038 <puts@GLIBC_2.2.5>
    1076:	68 04 00 00 00       	push   $0x4
    107b:	e9 a0 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001080 <write@plt>:
    1080:	ff 25 ba 4f 00 00    	jmp    *0x4fba(%rip)        # 6040 <write@GLIBC_2.2.5>
    1086:	68 05 00 00 00       	push   $0x5
    108b:	e9 90 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001090 <strlen@plt>:
    1090:	ff 25 b2 4f 00 00    	jmp    *0x4fb2(%rip)        # 6048 <strlen@GLIBC_2.2.5>
    1096:	68 06 00 00 00       	push   $0x6
    109b:	e9 80 ff ff ff       	jmp    1020 <_init+0x20>

00000000000010a0 <__stack_chk_fail@plt>:
    10a0:	ff 25 aa 4f 00 00    	jmp    *0x4faa(%rip)        # 6050 <__stack_chk_fail@GLIBC_2.4>
    10a6:	68 07 00 00 00       	push   $0x7
    10ab:	e9 70 ff ff ff       	jmp    1020 <_init+0x20>

00000000000010b0 <printf@plt>:
    10b0:	ff 25 a2 4f 00 00    	jmp    *0x4fa2(%rip)        # 6058 <printf@GLIBC_2.2.5>
    10b6:	68 08 00 00 00       	push   $0x8
    10bb:	e9 60 ff ff ff       	jmp    1020 <_init+0x20>

00000000000010c0 <alarm@plt>:
    10c0:	ff 25 9a 4f 00 00    	jmp    *0x4f9a(%rip)        # 6060 <alarm@GLIBC_2.2.5>
    10c6:	68 09 00 00 00       	push   $0x9
    10cb:	e9 50 ff ff ff       	jmp    1020 <_init+0x20>

00000000000010d0 <close@plt>:
    10d0:	ff 25 92 4f 00 00    	jmp    *0x4f92(%rip)        # 6068 <close@GLIBC_2.2.5>
    10d6:	68 0a 00 00 00       	push   $0xa
    10db:	e9 40 ff ff ff       	jmp    1020 <_init+0x20>

00000000000010e0 <read@plt>:
    10e0:	ff 25 8a 4f 00 00    	jmp    *0x4f8a(%rip)        # 6070 <read@GLIBC_2.2.5>
    10e6:	68 0b 00 00 00       	push   $0xb
    10eb:	e9 30 ff ff ff       	jmp    1020 <_init+0x20>

00000000000010f0 <fgets@plt>:
    10f0:	ff 25 82 4f 00 00    	jmp    *0x4f82(%rip)        # 6078 <fgets@GLIBC_2.2.5>
    10f6:	68 0c 00 00 00       	push   $0xc
    10fb:	e9 20 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001100 <strcmp@plt>:
    1100:	ff 25 7a 4f 00 00    	jmp    *0x4f7a(%rip)        # 6080 <strcmp@GLIBC_2.2.5>
    1106:	68 0d 00 00 00       	push   $0xd
    110b:	e9 10 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001110 <signal@plt>:
    1110:	ff 25 72 4f 00 00    	jmp    *0x4f72(%rip)        # 6088 <signal@GLIBC_2.2.5>
    1116:	68 0e 00 00 00       	push   $0xe
    111b:	e9 00 ff ff ff       	jmp    1020 <_init+0x20>

0000000000001120 <gethostbyname@plt>:
    1120:	ff 25 6a 4f 00 00    	jmp    *0x4f6a(%rip)        # 6090 <gethostbyname@GLIBC_2.2.5>
    1126:	68 0f 00 00 00       	push   $0xf
    112b:	e9 f0 fe ff ff       	jmp    1020 <_init+0x20>

0000000000001130 <fprintf@plt>:
    1130:	ff 25 62 4f 00 00    	jmp    *0x4f62(%rip)        # 6098 <fprintf@GLIBC_2.2.5>
    1136:	68 10 00 00 00       	push   $0x10
    113b:	e9 e0 fe ff ff       	jmp    1020 <_init+0x20>

0000000000001140 <fflush@plt>:
    1140:	ff 25 5a 4f 00 00    	jmp    *0x4f5a(%rip)        # 60a0 <fflush@GLIBC_2.2.5>
    1146:	68 11 00 00 00       	push   $0x11
    114b:	e9 d0 fe ff ff       	jmp    1020 <_init+0x20>

0000000000001150 <__isoc99_sscanf@plt>:
    1150:	ff 25 52 4f 00 00    	jmp    *0x4f52(%rip)        # 60a8 <__isoc99_sscanf@GLIBC_2.7>
    1156:	68 12 00 00 00       	push   $0x12
    115b:	e9 c0 fe ff ff       	jmp    1020 <_init+0x20>

0000000000001160 <memmove@plt>:
    1160:	ff 25 4a 4f 00 00    	jmp    *0x4f4a(%rip)        # 60b0 <memmove@GLIBC_2.2.5>
    1166:	68 13 00 00 00       	push   $0x13
    116b:	e9 b0 fe ff ff       	jmp    1020 <_init+0x20>

0000000000001170 <fopen@plt>:
    1170:	ff 25 42 4f 00 00    	jmp    *0x4f42(%rip)        # 60b8 <fopen@GLIBC_2.2.5>
    1176:	68 14 00 00 00       	push   $0x14
    117b:	e9 a0 fe ff ff       	jmp    1020 <_init+0x20>

0000000000001180 <sprintf@plt>:
    1180:	ff 25 3a 4f 00 00    	jmp    *0x4f3a(%rip)        # 60c0 <sprintf@GLIBC_2.2.5>
    1186:	68 15 00 00 00       	push   $0x15
    118b:	e9 90 fe ff ff       	jmp    1020 <_init+0x20>

0000000000001190 <exit@plt>:
    1190:	ff 25 32 4f 00 00    	jmp    *0x4f32(%rip)        # 60c8 <exit@GLIBC_2.2.5>
    1196:	68 16 00 00 00       	push   $0x16
    119b:	e9 80 fe ff ff       	jmp    1020 <_init+0x20>

00000000000011a0 <connect@plt>:
    11a0:	ff 25 2a 4f 00 00    	jmp    *0x4f2a(%rip)        # 60d0 <connect@GLIBC_2.2.5>
    11a6:	68 17 00 00 00       	push   $0x17
    11ab:	e9 70 fe ff ff       	jmp    1020 <_init+0x20>

00000000000011b0 <sleep@plt>:
    11b0:	ff 25 22 4f 00 00    	jmp    *0x4f22(%rip)        # 60d8 <sleep@GLIBC_2.2.5>
    11b6:	68 18 00 00 00       	push   $0x18
    11bb:	e9 60 fe ff ff       	jmp    1020 <_init+0x20>

00000000000011c0 <__ctype_b_loc@plt>:
    11c0:	ff 25 1a 4f 00 00    	jmp    *0x4f1a(%rip)        # 60e0 <__ctype_b_loc@GLIBC_2.3>
    11c6:	68 19 00 00 00       	push   $0x19
    11cb:	e9 50 fe ff ff       	jmp    1020 <_init+0x20>

00000000000011d0 <socket@plt>:
    11d0:	ff 25 12 4f 00 00    	jmp    *0x4f12(%rip)        # 60e8 <socket@GLIBC_2.2.5>
    11d6:	68 1a 00 00 00       	push   $0x1a
    11db:	e9 40 fe ff ff       	jmp    1020 <_init+0x20>

Disassembly of section .text:

00000000000011e0 <_start>:
    11e0:	f3 0f 1e fa          	endbr64
    11e4:	31 ed                	xor    %ebp,%ebp
    11e6:	49 89 d1             	mov    %rdx,%r9
    11e9:	5e                   	pop    %rsi
    11ea:	48 89 e2             	mov    %rsp,%rdx
    11ed:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    11f1:	50                   	push   %rax
    11f2:	54                   	push   %rsp
    11f3:	4c 8d 05 e6 1c 00 00 	lea    0x1ce6(%rip),%r8        # 2ee0 <__libc_csu_fini>
    11fa:	48 8d 0d 6f 1c 00 00 	lea    0x1c6f(%rip),%rcx        # 2e70 <__libc_csu_init>
    1201:	48 8d 3d d1 00 00 00 	lea    0xd1(%rip),%rdi        # 12d9 <main>
    1208:	ff 15 d2 4d 00 00    	call   *0x4dd2(%rip)        # 5fe0 <__libc_start_main@GLIBC_2.2.5>
    120e:	f4                   	hlt
    120f:	90                   	nop

0000000000001210 <deregister_tm_clones>:
    1210:	48 8d 3d 69 54 00 00 	lea    0x5469(%rip),%rdi        # 6680 <stdout@GLIBC_2.2.5>
    1217:	48 8d 05 62 54 00 00 	lea    0x5462(%rip),%rax        # 6680 <stdout@GLIBC_2.2.5>
    121e:	48 39 f8             	cmp    %rdi,%rax
    1221:	74 15                	je     1238 <deregister_tm_clones+0x28>
    1223:	48 8b 05 ae 4d 00 00 	mov    0x4dae(%rip),%rax        # 5fd8 <_ITM_deregisterTMCloneTable>
    122a:	48 85 c0             	test   %rax,%rax
    122d:	74 09                	je     1238 <deregister_tm_clones+0x28>
    122f:	ff e0                	jmp    *%rax
    1231:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    1238:	c3                   	ret
    1239:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001240 <register_tm_clones>:
    1240:	48 8d 3d 39 54 00 00 	lea    0x5439(%rip),%rdi        # 6680 <stdout@GLIBC_2.2.5>
    1247:	48 8d 35 32 54 00 00 	lea    0x5432(%rip),%rsi        # 6680 <stdout@GLIBC_2.2.5>
    124e:	48 29 fe             	sub    %rdi,%rsi
    1251:	48 89 f0             	mov    %rsi,%rax
    1254:	48 c1 ee 3f          	shr    $0x3f,%rsi
    1258:	48 c1 f8 03          	sar    $0x3,%rax
    125c:	48 01 c6             	add    %rax,%rsi
    125f:	48 d1 fe             	sar    $1,%rsi
    1262:	74 14                	je     1278 <register_tm_clones+0x38>
    1264:	48 8b 05 85 4d 00 00 	mov    0x4d85(%rip),%rax        # 5ff0 <_ITM_registerTMCloneTable>
    126b:	48 85 c0             	test   %rax,%rax
    126e:	74 08                	je     1278 <register_tm_clones+0x38>
    1270:	ff e0                	jmp    *%rax
    1272:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    1278:	c3                   	ret
    1279:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001280 <__do_global_dtors_aux>:
    1280:	f3 0f 1e fa          	endbr64
    1284:	80 3d 1d 54 00 00 00 	cmpb   $0x0,0x541d(%rip)        # 66a8 <completed.0>
    128b:	75 33                	jne    12c0 <__do_global_dtors_aux+0x40>
    128d:	55                   	push   %rbp
    128e:	48 83 3d 62 4d 00 00 	cmpq   $0x0,0x4d62(%rip)        # 5ff8 <__cxa_finalize@GLIBC_2.2.5>
    1295:	00 
    1296:	48 89 e5             	mov    %rsp,%rbp
    1299:	74 0d                	je     12a8 <__do_global_dtors_aux+0x28>
    129b:	48 8b 3d 66 4e 00 00 	mov    0x4e66(%rip),%rdi        # 6108 <__dso_handle>
    12a2:	ff 15 50 4d 00 00    	call   *0x4d50(%rip)        # 5ff8 <__cxa_finalize@GLIBC_2.2.5>
    12a8:	e8 63 ff ff ff       	call   1210 <deregister_tm_clones>
    12ad:	c6 05 f4 53 00 00 01 	movb   $0x1,0x53f4(%rip)        # 66a8 <completed.0>
    12b4:	5d                   	pop    %rbp
    12b5:	c3                   	ret
    12b6:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    12bd:	00 00 00 
    12c0:	c3                   	ret
    12c1:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
    12c8:	00 00 00 00 
    12cc:	0f 1f 40 00          	nopl   0x0(%rax)

00000000000012d0 <frame_dummy>:
    12d0:	f3 0f 1e fa          	endbr64
    12d4:	e9 67 ff ff ff       	jmp    1240 <register_tm_clones>

00000000000012d9 <main>:
    12d9:	53                   	push   %rbx
    12da:	83 ff 01             	cmp    $0x1,%edi
    12dd:	0f 84 04 01 00 00    	je     13e7 <main+0x10e>
    12e3:	48 89 f3             	mov    %rsi,%rbx
    12e6:	83 ff 02             	cmp    $0x2,%edi
    12e9:	0f 85 28 01 00 00    	jne    1417 <main+0x13e>
    12ef:	48 8b 7e 08          	mov    0x8(%rsi),%rdi
    12f3:	48 8d 35 0a 1d 00 00 	lea    0x1d0a(%rip),%rsi        # 3004 <_IO_stdin_used+0x4>
    12fa:	e8 71 fe ff ff       	call   1170 <fopen@plt>
    12ff:	48 89 05 aa 53 00 00 	mov    %rax,0x53aa(%rip)        # 66b0 <infile>
    1306:	48 85 c0             	test   %rax,%rax
    1309:	0f 84 eb 00 00 00    	je     13fa <main+0x121>
    130f:	e8 65 0a 00 00       	call   1d79 <initialize_bomb>
    1314:	48 8d 3d 65 1d 00 00 	lea    0x1d65(%rip),%rdi        # 3080 <_IO_stdin_used+0x80>
    131b:	e8 50 fd ff ff       	call   1070 <puts@plt>
    1320:	48 8d 3d 99 1d 00 00 	lea    0x1d99(%rip),%rdi        # 30c0 <_IO_stdin_used+0xc0>
    1327:	e8 44 fd ff ff       	call   1070 <puts@plt>
    132c:	e8 52 0d 00 00       	call   2083 <read_line>
    1331:	48 89 c7             	mov    %rax,%rdi
    1334:	e8 fc 00 00 00       	call   1435 <phase_1>
    1339:	e8 7f 0e 00 00       	call   21bd <phase_defused>
    133e:	48 8d 3d ab 1d 00 00 	lea    0x1dab(%rip),%rdi        # 30f0 <_IO_stdin_used+0xf0>
    1345:	e8 26 fd ff ff       	call   1070 <puts@plt>
    134a:	e8 34 0d 00 00       	call   2083 <read_line>
    134f:	48 89 c7             	mov    %rax,%rdi
    1352:	e8 fe 00 00 00       	call   1455 <phase_2>
    1357:	e8 61 0e 00 00       	call   21bd <phase_defused>
    135c:	48 8d 3d da 1c 00 00 	lea    0x1cda(%rip),%rdi        # 303d <_IO_stdin_used+0x3d>
    1363:	e8 08 fd ff ff       	call   1070 <puts@plt>
    1368:	e8 16 0d 00 00       	call   2083 <read_line>
    136d:	48 89 c7             	mov    %rax,%rdi
    1370:	e8 cf 01 00 00       	call   1544 <phase_3>
    1375:	e8 43 0e 00 00       	call   21bd <phase_defused>
    137a:	48 8d 3d cd 1c 00 00 	lea    0x1ccd(%rip),%rdi        # 304e <_IO_stdin_used+0x4e>
    1381:	e8 ea fc ff ff       	call   1070 <puts@plt>
    1386:	48 8d 3d 93 1d 00 00 	lea    0x1d93(%rip),%rdi        # 3120 <_IO_stdin_used+0x120>
    138d:	e8 de fc ff ff       	call   1070 <puts@plt>
    1392:	e8 ec 0c 00 00       	call   2083 <read_line>
    1397:	48 89 c7             	mov    %rax,%rdi
    139a:	e8 ea 03 00 00       	call   1789 <phase_4>
    139f:	e8 19 0e 00 00       	call   21bd <phase_defused>
    13a4:	48 8d 3d ad 1d 00 00 	lea    0x1dad(%rip),%rdi        # 3158 <_IO_stdin_used+0x158>
    13ab:	e8 c0 fc ff ff       	call   1070 <puts@plt>
    13b0:	e8 ce 0c 00 00       	call   2083 <read_line>
    13b5:	48 89 c7             	mov    %rax,%rdi
    13b8:	e8 8a 04 00 00       	call   1847 <phase_5>
    13bd:	e8 fb 0d 00 00       	call   21bd <phase_defused>
    13c2:	48 8d 3d 94 1c 00 00 	lea    0x1c94(%rip),%rdi        # 305d <_IO_stdin_used+0x5d>
    13c9:	e8 a2 fc ff ff       	call   1070 <puts@plt>
    13ce:	e8 b0 0c 00 00       	call   2083 <read_line>
    13d3:	48 89 c7             	mov    %rax,%rdi
    13d6:	e8 03 05 00 00       	call   18de <phase_6>
    13db:	e8 dd 0d 00 00       	call   21bd <phase_defused>
    13e0:	b8 00 00 00 00       	mov    $0x0,%eax
    13e5:	5b                   	pop    %rbx
    13e6:	c3                   	ret
    13e7:	48 8b 05 a2 52 00 00 	mov    0x52a2(%rip),%rax        # 6690 <stdin@GLIBC_2.2.5>
    13ee:	48 89 05 bb 52 00 00 	mov    %rax,0x52bb(%rip)        # 66b0 <infile>
    13f5:	e9 15 ff ff ff       	jmp    130f <main+0x36>
    13fa:	48 8b 53 08          	mov    0x8(%rbx),%rdx
    13fe:	48 8b 33             	mov    (%rbx),%rsi
    1401:	48 8d 3d fe 1b 00 00 	lea    0x1bfe(%rip),%rdi        # 3006 <_IO_stdin_used+0x6>
    1408:	e8 a3 fc ff ff       	call   10b0 <printf@plt>
    140d:	bf 08 00 00 00       	mov    $0x8,%edi
    1412:	e8 79 fd ff ff       	call   1190 <exit@plt>
    1417:	48 8b 36             	mov    (%rsi),%rsi
    141a:	48 8d 3d 02 1c 00 00 	lea    0x1c02(%rip),%rdi        # 3023 <_IO_stdin_used+0x23>
    1421:	b8 00 00 00 00       	mov    $0x0,%eax
    1426:	e8 85 fc ff ff       	call   10b0 <printf@plt>
    142b:	bf 08 00 00 00       	mov    $0x8,%edi
    1430:	e8 5b fd ff ff       	call   1190 <exit@plt>

0000000000001435 <phase_1>:
    1435:	48 83 ec 08          	sub    $0x8,%rsp
    1439:	48 8d 35 40 1d 00 00 	lea    0x1d40(%rip),%rsi        # 3180 <_IO_stdin_used+0x180>
    1440:	e8 d8 08 00 00       	call   1d1d <strings_not_equal>
    1445:	85 c0                	test   %eax,%eax
    1447:	75 05                	jne    144e <phase_1+0x19>
    1449:	48 83 c4 08          	add    $0x8,%rsp
    144d:	c3                   	ret
    144e:	e8 2f 0b 00 00       	call   1f82 <explode_bomb>
    1453:	eb f4                	jmp    1449 <phase_1+0x14>

0000000000001455 <phase_2>:
    1455:	55                   	push   %rbp
    1456:	53                   	push   %rbx
    1457:	48 83 ec 38          	sub    $0x38,%rsp
    145b:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1462:	00 00 
    1464:	48 89 44 24 28       	mov    %rax,0x28(%rsp)
    1469:	31 c0                	xor    %eax,%eax
    146b:	48 89 e2             	mov    %rsp,%rdx
    146e:	48 8d 4c 24 04       	lea    0x4(%rsp),%rcx
    1473:	4c 8d 4c 24 0c       	lea    0xc(%rsp),%r9
    1478:	4c 8d 44 24 08       	lea    0x8(%rsp),%r8
    147d:	48 8d 35 8d 21 00 00 	lea    0x218d(%rip),%rsi        # 3611 <array.0+0x391>
    1484:	e8 c7 fc ff ff       	call   1150 <__isoc99_sscanf@plt>
    1489:	83 f8 04             	cmp    $0x4,%eax
    148c:	75 14                	jne    14a2 <phase_2+0x4d>
    148e:	48 8d 3d ab 4c 00 00 	lea    0x4cab(%rip),%rdi        # 6140 <matA.3>
    1495:	48 8d 5c 24 10       	lea    0x10(%rsp),%rbx
    149a:	41 bb 00 00 00 00    	mov    $0x0,%r11d
    14a0:	eb 19                	jmp    14bb <phase_2+0x66>
    14a2:	e8 db 0a 00 00       	call   1f82 <explode_bomb>
    14a7:	eb e5                	jmp    148e <phase_2+0x39>
    14a9:	41 83 c3 01          	add    $0x1,%r11d
    14ad:	48 83 c7 0c          	add    $0xc,%rdi
    14b1:	48 83 c3 08          	add    $0x8,%rbx
    14b5:	41 83 fb 02          	cmp    $0x2,%r11d
    14b9:	74 47                	je     1502 <phase_2+0xad>
    14bb:	48 8d 35 5e 4c 00 00 	lea    0x4c5e(%rip),%rsi        # 6120 <matB.2>
    14c2:	49 89 d9             	mov    %rbx,%r9
    14c5:	41 b8 00 00 00 00    	mov    $0x0,%r8d
    14cb:	4d 89 ca             	mov    %r9,%r10
    14ce:	b8 00 00 00 00       	mov    $0x0,%eax
    14d3:	b9 00 00 00 00       	mov    $0x0,%ecx
    14d8:	8b 14 87             	mov    (%rdi,%rax,4),%edx
    14db:	0f af 14 c6          	imul   (%rsi,%rax,8),%edx
    14df:	01 d1                	add    %edx,%ecx
    14e1:	48 83 c0 01          	add    $0x1,%rax
    14e5:	48 83 f8 03          	cmp    $0x3,%rax
    14e9:	75 ed                	jne    14d8 <phase_2+0x83>
    14eb:	41 89 0a             	mov    %ecx,(%r10)
    14ee:	41 83 c0 01          	add    $0x1,%r8d
    14f2:	49 83 c1 04          	add    $0x4,%r9
    14f6:	48 83 c6 04          	add    $0x4,%rsi
    14fa:	41 83 f8 02          	cmp    $0x2,%r8d
    14fe:	75 cb                	jne    14cb <phase_2+0x76>
    1500:	eb a7                	jmp    14a9 <phase_2+0x54>
    1502:	bb 00 00 00 00       	mov    $0x0,%ebx
    1507:	48 8d 6c 24 10       	lea    0x10(%rsp),%rbp
    150c:	eb 0a                	jmp    1518 <phase_2+0xc3>
    150e:	48 83 c3 04          	add    $0x4,%rbx
    1512:	48 83 fb 10          	cmp    $0x10,%rbx
    1516:	74 10                	je     1528 <phase_2+0xd3>
    1518:	8b 44 1d 00          	mov    0x0(%rbp,%rbx,1),%eax
    151c:	39 04 1c             	cmp    %eax,(%rsp,%rbx,1)
    151f:	74 ed                	je     150e <phase_2+0xb9>
    1521:	e8 5c 0a 00 00       	call   1f82 <explode_bomb>
    1526:	eb e6                	jmp    150e <phase_2+0xb9>
    1528:	48 8b 44 24 28       	mov    0x28(%rsp),%rax
    152d:	64 48 2b 04 25 28 00 	sub    %fs:0x28,%rax
    1534:	00 00 
    1536:	75 07                	jne    153f <phase_2+0xea>
    1538:	48 83 c4 38          	add    $0x38,%rsp
    153c:	5b                   	pop    %rbx
    153d:	5d                   	pop    %rbp
    153e:	c3                   	ret
    153f:	e8 5c fb ff ff       	call   10a0 <__stack_chk_fail@plt>

0000000000001544 <phase_3>:
    1544:	48 83 ec 28          	sub    $0x28,%rsp
    1548:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    154f:	00 00 
    1551:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
    1556:	31 c0                	xor    %eax,%eax
    1558:	48 8d 4c 24 0f       	lea    0xf(%rsp),%rcx
    155d:	48 8d 54 24 10       	lea    0x10(%rsp),%rdx
    1562:	4c 8d 44 24 14       	lea    0x14(%rsp),%r8
    1567:	48 8d 35 b7 1c 00 00 	lea    0x1cb7(%rip),%rsi        # 3225 <_IO_stdin_used+0x225>
    156e:	e8 dd fb ff ff       	call   1150 <__isoc99_sscanf@plt>
    1573:	83 f8 02             	cmp    $0x2,%eax
    1576:	7e 29                	jle    15a1 <phase_3+0x5d>
    1578:	8b 05 92 4b 00 00    	mov    0x4b92(%rip),%eax        # 6110 <mask.1>
    157e:	30 44 24 0f          	xor    %al,0xf(%rsp)
    1582:	83 7c 24 10 07       	cmpl   $0x7,0x10(%rsp)
    1587:	0f 87 0c 01 00 00    	ja     1699 <phase_3+0x155>
    158d:	8b 44 24 10          	mov    0x10(%rsp),%eax
    1591:	48 8d 15 c8 1c 00 00 	lea    0x1cc8(%rip),%rdx        # 3260 <_IO_stdin_used+0x260>
    1598:	48 63 04 82          	movslq (%rdx,%rax,4),%rax
    159c:	48 01 d0             	add    %rdx,%rax
    159f:	ff e0                	jmp    *%rax
    15a1:	e8 dc 09 00 00       	call   1f82 <explode_bomb>
    15a6:	eb d0                	jmp    1578 <phase_3+0x34>
    15a8:	b8 74 00 00 00       	mov    $0x74,%eax
    15ad:	81 7c 24 14 f7 00 00 	cmpl   $0xf7,0x14(%rsp)
    15b4:	00 
    15b5:	0f 84 e8 00 00 00    	je     16a3 <phase_3+0x15f>
    15bb:	e8 c2 09 00 00       	call   1f82 <explode_bomb>
    15c0:	b8 74 00 00 00       	mov    $0x74,%eax
    15c5:	e9 d9 00 00 00       	jmp    16a3 <phase_3+0x15f>
    15ca:	b8 6a 00 00 00       	mov    $0x6a,%eax
    15cf:	81 7c 24 14 be 02 00 	cmpl   $0x2be,0x14(%rsp)
    15d6:	00 
    15d7:	0f 84 c6 00 00 00    	je     16a3 <phase_3+0x15f>
    15dd:	e8 a0 09 00 00       	call   1f82 <explode_bomb>
    15e2:	b8 6a 00 00 00       	mov    $0x6a,%eax
    15e7:	e9 b7 00 00 00       	jmp    16a3 <phase_3+0x15f>
    15ec:	b8 70 00 00 00       	mov    $0x70,%eax
    15f1:	81 7c 24 14 a0 01 00 	cmpl   $0x1a0,0x14(%rsp)
    15f8:	00 
    15f9:	0f 84 a4 00 00 00    	je     16a3 <phase_3+0x15f>
    15ff:	e8 7e 09 00 00       	call   1f82 <explode_bomb>
    1604:	b8 70 00 00 00       	mov    $0x70,%eax
    1609:	e9 95 00 00 00       	jmp    16a3 <phase_3+0x15f>
    160e:	b8 6a 00 00 00       	mov    $0x6a,%eax
    1613:	81 7c 24 14 b5 00 00 	cmpl   $0xb5,0x14(%rsp)
    161a:	00 
    161b:	0f 84 82 00 00 00    	je     16a3 <phase_3+0x15f>
    1621:	e8 5c 09 00 00       	call   1f82 <explode_bomb>
    1626:	b8 6a 00 00 00       	mov    $0x6a,%eax
    162b:	eb 76                	jmp    16a3 <phase_3+0x15f>
    162d:	b8 77 00 00 00       	mov    $0x77,%eax
    1632:	81 7c 24 14 45 01 00 	cmpl   $0x145,0x14(%rsp)
    1639:	00 
    163a:	74 67                	je     16a3 <phase_3+0x15f>
    163c:	e8 41 09 00 00       	call   1f82 <explode_bomb>
    1641:	b8 77 00 00 00       	mov    $0x77,%eax
    1646:	eb 5b                	jmp    16a3 <phase_3+0x15f>
    1648:	b8 69 00 00 00       	mov    $0x69,%eax
    164d:	81 7c 24 14 a7 03 00 	cmpl   $0x3a7,0x14(%rsp)
    1654:	00 
    1655:	74 4c                	je     16a3 <phase_3+0x15f>
    1657:	e8 26 09 00 00       	call   1f82 <explode_bomb>
    165c:	b8 69 00 00 00       	mov    $0x69,%eax
    1661:	eb 40                	jmp    16a3 <phase_3+0x15f>
    1663:	b8 6f 00 00 00       	mov    $0x6f,%eax
    1668:	81 7c 24 14 0c 01 00 	cmpl   $0x10c,0x14(%rsp)
    166f:	00 
    1670:	74 31                	je     16a3 <phase_3+0x15f>
    1672:	e8 0b 09 00 00       	call   1f82 <explode_bomb>
    1677:	b8 6f 00 00 00       	mov    $0x6f,%eax
    167c:	eb 25                	jmp    16a3 <phase_3+0x15f>
    167e:	b8 6a 00 00 00       	mov    $0x6a,%eax
    1683:	81 7c 24 14 02 02 00 	cmpl   $0x202,0x14(%rsp)
    168a:	00 
    168b:	74 16                	je     16a3 <phase_3+0x15f>
    168d:	e8 f0 08 00 00       	call   1f82 <explode_bomb>
    1692:	b8 6a 00 00 00       	mov    $0x6a,%eax
    1697:	eb 0a                	jmp    16a3 <phase_3+0x15f>
    1699:	e8 e4 08 00 00       	call   1f82 <explode_bomb>
    169e:	b8 61 00 00 00       	mov    $0x61,%eax
    16a3:	38 44 24 0f          	cmp    %al,0xf(%rsp)
    16a7:	75 15                	jne    16be <phase_3+0x17a>
    16a9:	48 8b 44 24 18       	mov    0x18(%rsp),%rax
    16ae:	64 48 2b 04 25 28 00 	sub    %fs:0x28,%rax
    16b5:	00 00 
    16b7:	75 0c                	jne    16c5 <phase_3+0x181>
    16b9:	48 83 c4 28          	add    $0x28,%rsp
    16bd:	c3                   	ret
    16be:	e8 bf 08 00 00       	call   1f82 <explode_bomb>
    16c3:	eb e4                	jmp    16a9 <phase_3+0x165>
    16c5:	e8 d6 f9 ff ff       	call   10a0 <__stack_chk_fail@plt>

00000000000016ca <func4_1>:
    16ca:	b8 00 00 00 00       	mov    $0x0,%eax
    16cf:	85 ff                	test   %edi,%edi
    16d1:	7e 1c                	jle    16ef <func4_1+0x25>
    16d3:	89 f8                	mov    %edi,%eax
    16d5:	83 ff 01             	cmp    $0x1,%edi
    16d8:	74 15                	je     16ef <func4_1+0x25>
    16da:	48 83 ec 08          	sub    $0x8,%rsp
    16de:	83 ef 01             	sub    $0x1,%edi
    16e1:	e8 e4 ff ff ff       	call   16ca <func4_1>
    16e6:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax
    16ea:	48 83 c4 08          	add    $0x8,%rsp
    16ee:	c3                   	ret
    16ef:	c3                   	ret

00000000000016f0 <func4_2>:
    16f0:	41 57                	push   %r15
    16f2:	41 56                	push   %r14
    16f4:	41 55                	push   %r13
    16f6:	41 54                	push   %r12
    16f8:	55                   	push   %rbp
    16f9:	53                   	push   %rbx
    16fa:	48 83 ec 08          	sub    $0x8,%rsp
    16fe:	41 89 d4             	mov    %edx,%r12d
    1701:	41 89 cd             	mov    %ecx,%r13d
    1704:	4c 89 cd             	mov    %r9,%rbp
    1707:	83 ff 01             	cmp    $0x1,%edi
    170a:	74 2a                	je     1736 <func4_2+0x46>
    170c:	89 f3                	mov    %esi,%ebx
    170e:	45 89 c6             	mov    %r8d,%r14d
    1711:	44 8d 7f ff          	lea    -0x1(%rdi),%r15d
    1715:	44 89 ff             	mov    %r15d,%edi
    1718:	e8 ad ff ff ff       	call   16ca <func4_1>
    171d:	39 d8                	cmp    %ebx,%eax
    171f:	7d 2f                	jge    1750 <func4_2+0x60>
    1721:	8d 50 01             	lea    0x1(%rax),%edx
    1724:	39 da                	cmp    %ebx,%edx
    1726:	75 43                	jne    176b <func4_2+0x7b>
    1728:	44 88 65 00          	mov    %r12b,0x0(%rbp)
    172c:	44 88 6d 01          	mov    %r13b,0x1(%rbp)
    1730:	c6 45 02 00          	movb   $0x0,0x2(%rbp)
    1734:	eb 0b                	jmp    1741 <func4_2+0x51>
    1736:	88 55 00             	mov    %dl,0x0(%rbp)
    1739:	88 4d 01             	mov    %cl,0x1(%rbp)
    173c:	41 c6 41 02 00       	movb   $0x0,0x2(%r9)
    1741:	48 83 c4 08          	add    $0x8,%rsp
    1745:	5b                   	pop    %rbx
    1746:	5d                   	pop    %rbp
    1747:	41 5c                	pop    %r12
    1749:	41 5d                	pop    %r13
    174b:	41 5e                	pop    %r14
    174d:	41 5f                	pop    %r15
    174f:	c3                   	ret
    1750:	41 0f be ce          	movsbl %r14b,%ecx
    1754:	41 0f be d4          	movsbl %r12b,%edx
    1758:	49 89 e9             	mov    %rbp,%r9
    175b:	45 0f be c5          	movsbl %r13b,%r8d
    175f:	89 de                	mov    %ebx,%esi
    1761:	44 89 ff             	mov    %r15d,%edi
    1764:	e8 87 ff ff ff       	call   16f0 <func4_2>
    1769:	eb d6                	jmp    1741 <func4_2+0x51>
    176b:	41 0f be cd          	movsbl %r13b,%ecx
    176f:	41 0f be d6          	movsbl %r14b,%edx
    1773:	29 c3                	sub    %eax,%ebx
    1775:	8d 73 ff             	lea    -0x1(%rbx),%esi
    1778:	49 89 e9             	mov    %rbp,%r9
    177b:	45 0f be c4          	movsbl %r12b,%r8d
    177f:	44 89 ff             	mov    %r15d,%edi
    1782:	e8 69 ff ff ff       	call   16f0 <func4_2>
    1787:	eb b8                	jmp    1741 <func4_2+0x51>

0000000000001789 <phase_4>:
    1789:	53                   	push   %rbx
    178a:	48 83 ec 20          	sub    $0x20,%rsp
    178e:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1795:	00 00 
    1797:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
    179c:	31 c0                	xor    %eax,%eax
    179e:	48 8d 4c 24 10       	lea    0x10(%rsp),%rcx
    17a3:	48 8d 54 24 0c       	lea    0xc(%rsp),%rdx
    17a8:	48 8d 35 7f 1a 00 00 	lea    0x1a7f(%rip),%rsi        # 322e <_IO_stdin_used+0x22e>
    17af:	e8 9c f9 ff ff       	call   1150 <__isoc99_sscanf@plt>
    17b4:	83 f8 02             	cmp    $0x2,%eax
    17b7:	75 6d                	jne    1826 <phase_4+0x9d>
    17b9:	bf 05 00 00 00       	mov    $0x5,%edi
    17be:	e8 07 ff ff ff       	call   16ca <func4_1>
    17c3:	39 44 24 0c          	cmp    %eax,0xc(%rsp)
    17c7:	75 64                	jne    182d <phase_4+0xa4>
    17c9:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
    17ce:	e8 2d 05 00 00       	call   1d00 <string_length>
    17d3:	83 f8 02             	cmp    $0x2,%eax
    17d6:	75 5c                	jne    1834 <phase_4+0xab>
    17d8:	48 8d 5c 24 14       	lea    0x14(%rsp),%rbx
    17dd:	49 89 d9             	mov    %rbx,%r9
    17e0:	41 b8 42 00 00 00    	mov    $0x42,%r8d
    17e6:	b9 43 00 00 00       	mov    $0x43,%ecx
    17eb:	ba 41 00 00 00       	mov    $0x41,%edx
    17f0:	be 0f 00 00 00       	mov    $0xf,%esi
    17f5:	bf 05 00 00 00       	mov    $0x5,%edi
    17fa:	e8 f1 fe ff ff       	call   16f0 <func4_2>
    17ff:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
    1804:	48 89 de             	mov    %rbx,%rsi
    1807:	e8 11 05 00 00       	call   1d1d <strings_not_equal>
    180c:	85 c0                	test   %eax,%eax
    180e:	75 2b                	jne    183b <phase_4+0xb2>
    1810:	48 8b 44 24 18       	mov    0x18(%rsp),%rax
    1815:	64 48 2b 04 25 28 00 	sub    %fs:0x28,%rax
    181c:	00 00 
    181e:	75 22                	jne    1842 <phase_4+0xb9>
    1820:	48 83 c4 20          	add    $0x20,%rsp
    1824:	5b                   	pop    %rbx
    1825:	c3                   	ret
    1826:	e8 57 07 00 00       	call   1f82 <explode_bomb>
    182b:	eb 8c                	jmp    17b9 <phase_4+0x30>
    182d:	e8 50 07 00 00       	call   1f82 <explode_bomb>
    1832:	eb 95                	jmp    17c9 <phase_4+0x40>
    1834:	e8 49 07 00 00       	call   1f82 <explode_bomb>
    1839:	eb 9d                	jmp    17d8 <phase_4+0x4f>
    183b:	e8 42 07 00 00       	call   1f82 <explode_bomb>
    1840:	eb ce                	jmp    1810 <phase_4+0x87>
    1842:	e8 59 f8 ff ff       	call   10a0 <__stack_chk_fail@plt>

0000000000001847 <phase_5>:
    1847:	48 83 ec 18          	sub    $0x18,%rsp
    184b:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1852:	00 00 
    1854:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
    1859:	31 c0                	xor    %eax,%eax
    185b:	48 8d 4c 24 04       	lea    0x4(%rsp),%rcx
    1860:	48 89 e2             	mov    %rsp,%rdx
    1863:	48 8d 35 ad 1d 00 00 	lea    0x1dad(%rip),%rsi        # 3617 <array.0+0x397>
    186a:	e8 e1 f8 ff ff       	call   1150 <__isoc99_sscanf@plt>
    186f:	83 f8 01             	cmp    $0x1,%eax
    1872:	7e 06                	jle    187a <phase_5+0x33>
    1874:	83 3c 24 00          	cmpl   $0x0,(%rsp)
    1878:	78 05                	js     187f <phase_5+0x38>
    187a:	e8 03 07 00 00       	call   1f82 <explode_bomb>
    187f:	8b 04 24             	mov    (%rsp),%eax
    1882:	83 e0 0f             	and    $0xf,%eax
    1885:	89 04 24             	mov    %eax,(%rsp)
    1888:	83 f8 0f             	cmp    $0xf,%eax
    188b:	74 32                	je     18bf <phase_5+0x78>
    188d:	b9 00 00 00 00       	mov    $0x0,%ecx
    1892:	ba 00 00 00 00       	mov    $0x0,%edx
    1897:	48 8d 35 e2 19 00 00 	lea    0x19e2(%rip),%rsi        # 3280 <array.0>
    189e:	83 c2 01             	add    $0x1,%edx
    18a1:	48 98                	cltq
    18a3:	8b 04 86             	mov    (%rsi,%rax,4),%eax
    18a6:	01 c1                	add    %eax,%ecx
    18a8:	83 f8 0f             	cmp    $0xf,%eax
    18ab:	75 f1                	jne    189e <phase_5+0x57>
    18ad:	c7 04 24 0f 00 00 00 	movl   $0xf,(%rsp)
    18b4:	83 fa 09             	cmp    $0x9,%edx
    18b7:	75 06                	jne    18bf <phase_5+0x78>
    18b9:	39 4c 24 04          	cmp    %ecx,0x4(%rsp)
    18bd:	74 05                	je     18c4 <phase_5+0x7d>
    18bf:	e8 be 06 00 00       	call   1f82 <explode_bomb>
    18c4:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
    18c9:	64 48 2b 04 25 28 00 	sub    %fs:0x28,%rax
    18d0:	00 00 
    18d2:	75 05                	jne    18d9 <phase_5+0x92>
    18d4:	48 83 c4 18          	add    $0x18,%rsp
    18d8:	c3                   	ret
    18d9:	e8 c2 f7 ff ff       	call   10a0 <__stack_chk_fail@plt>

00000000000018de <phase_6>:
    18de:	41 56                	push   %r14
    18e0:	41 55                	push   %r13
    18e2:	41 54                	push   %r12
    18e4:	55                   	push   %rbp
    18e5:	53                   	push   %rbx
    18e6:	48 83 ec 60          	sub    $0x60,%rsp
    18ea:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    18f1:	00 00 
    18f3:	48 89 44 24 58       	mov    %rax,0x58(%rsp)
    18f8:	31 c0                	xor    %eax,%eax
    18fa:	49 89 e5             	mov    %rsp,%r13
    18fd:	4c 89 ee             	mov    %r13,%rsi
    1900:	e8 3d 07 00 00       	call   2042 <read_six_numbers>
    1905:	41 be 01 00 00 00    	mov    $0x1,%r14d
    190b:	49 89 e4             	mov    %rsp,%r12
    190e:	eb 28                	jmp    1938 <phase_6+0x5a>
    1910:	e8 6d 06 00 00       	call   1f82 <explode_bomb>
    1915:	eb 30                	jmp    1947 <phase_6+0x69>
    1917:	48 83 c3 01          	add    $0x1,%rbx
    191b:	83 fb 05             	cmp    $0x5,%ebx
    191e:	7f 10                	jg     1930 <phase_6+0x52>
    1920:	41 8b 04 9c          	mov    (%r12,%rbx,4),%eax
    1924:	39 45 00             	cmp    %eax,0x0(%rbp)
    1927:	75 ee                	jne    1917 <phase_6+0x39>
    1929:	e8 54 06 00 00       	call   1f82 <explode_bomb>
    192e:	eb e7                	jmp    1917 <phase_6+0x39>
    1930:	49 83 c6 01          	add    $0x1,%r14
    1934:	49 83 c5 04          	add    $0x4,%r13
    1938:	4c 89 ed             	mov    %r13,%rbp
    193b:	41 8b 45 00          	mov    0x0(%r13),%eax
    193f:	83 e8 01             	sub    $0x1,%eax
    1942:	83 f8 05             	cmp    $0x5,%eax
    1945:	77 c9                	ja     1910 <phase_6+0x32>
    1947:	41 83 fe 05          	cmp    $0x5,%r14d
    194b:	7f 05                	jg     1952 <phase_6+0x74>
    194d:	4c 89 f3             	mov    %r14,%rbx
    1950:	eb ce                	jmp    1920 <phase_6+0x42>
    1952:	be 00 00 00 00       	mov    $0x0,%esi
    1957:	8b 0c b4             	mov    (%rsp,%rsi,4),%ecx
    195a:	b8 01 00 00 00       	mov    $0x1,%eax
    195f:	48 8d 15 ba 48 00 00 	lea    0x48ba(%rip),%rdx        # 6220 <node1>
    1966:	83 f9 01             	cmp    $0x1,%ecx
    1969:	7e 0b                	jle    1976 <phase_6+0x98>
    196b:	48 8b 52 08          	mov    0x8(%rdx),%rdx
    196f:	83 c0 01             	add    $0x1,%eax
    1972:	39 c8                	cmp    %ecx,%eax
    1974:	75 f5                	jne    196b <phase_6+0x8d>
    1976:	48 89 54 f4 20       	mov    %rdx,0x20(%rsp,%rsi,8)
    197b:	48 83 c6 01          	add    $0x1,%rsi
    197f:	48 83 fe 06          	cmp    $0x6,%rsi
    1983:	75 d2                	jne    1957 <phase_6+0x79>
    1985:	48 8b 5c 24 20       	mov    0x20(%rsp),%rbx
    198a:	48 8b 44 24 28       	mov    0x28(%rsp),%rax
    198f:	48 89 43 08          	mov    %rax,0x8(%rbx)
    1993:	48 8b 54 24 30       	mov    0x30(%rsp),%rdx
    1998:	48 89 50 08          	mov    %rdx,0x8(%rax)
    199c:	48 8b 44 24 38       	mov    0x38(%rsp),%rax
    19a1:	48 89 42 08          	mov    %rax,0x8(%rdx)
    19a5:	48 8b 54 24 40       	mov    0x40(%rsp),%rdx
    19aa:	48 89 50 08          	mov    %rdx,0x8(%rax)
    19ae:	48 8b 44 24 48       	mov    0x48(%rsp),%rax
    19b3:	48 89 42 08          	mov    %rax,0x8(%rdx)
    19b7:	48 c7 40 08 00 00 00 	movq   $0x0,0x8(%rax)
    19be:	00 
    19bf:	bd 05 00 00 00       	mov    $0x5,%ebp
    19c4:	eb 09                	jmp    19cf <phase_6+0xf1>
    19c6:	48 8b 5b 08          	mov    0x8(%rbx),%rbx
    19ca:	83 ed 01             	sub    $0x1,%ebp
    19cd:	74 11                	je     19e0 <phase_6+0x102>
    19cf:	48 8b 43 08          	mov    0x8(%rbx),%rax
    19d3:	8b 00                	mov    (%rax),%eax
    19d5:	39 03                	cmp    %eax,(%rbx)
    19d7:	7e ed                	jle    19c6 <phase_6+0xe8>
    19d9:	e8 a4 05 00 00       	call   1f82 <explode_bomb>
    19de:	eb e6                	jmp    19c6 <phase_6+0xe8>
    19e0:	48 8b 44 24 58       	mov    0x58(%rsp),%rax
    19e5:	64 48 2b 04 25 28 00 	sub    %fs:0x28,%rax
    19ec:	00 00 
    19ee:	75 0d                	jne    19fd <phase_6+0x11f>
    19f0:	48 83 c4 60          	add    $0x60,%rsp
    19f4:	5b                   	pop    %rbx
    19f5:	5d                   	pop    %rbp
    19f6:	41 5c                	pop    %r12
    19f8:	41 5d                	pop    %r13
    19fa:	41 5e                	pop    %r14
    19fc:	c3                   	ret
    19fd:	e8 9e f6 ff ff       	call   10a0 <__stack_chk_fail@plt>

0000000000001a02 <func7>:
    1a02:	48 81 ec 98 00 00 00 	sub    $0x98,%rsp
    1a09:	89 f0                	mov    %esi,%eax
    1a0b:	41 89 c9             	mov    %ecx,%r9d
    1a0e:	64 48 8b 0c 25 28 00 	mov    %fs:0x28,%rcx
    1a15:	00 00 
    1a17:	48 89 8c 24 88 00 00 	mov    %rcx,0x88(%rsp)
    1a1e:	00 
    1a1f:	31 c9                	xor    %ecx,%ecx
    1a21:	c7 04 24 fe ff ff ff 	movl   $0xfffffffe,(%rsp)
    1a28:	c7 44 24 04 ff ff ff 	movl   $0xffffffff,0x4(%rsp)
    1a2f:	ff 
    1a30:	c7 44 24 08 01 00 00 	movl   $0x1,0x8(%rsp)
    1a37:	00 
    1a38:	c7 44 24 0c 02 00 00 	movl   $0x2,0xc(%rsp)
    1a3f:	00 
    1a40:	c7 44 24 10 02 00 00 	movl   $0x2,0x10(%rsp)
    1a47:	00 
    1a48:	c7 44 24 14 01 00 00 	movl   $0x1,0x14(%rsp)
    1a4f:	00 
    1a50:	c7 44 24 18 ff ff ff 	movl   $0xffffffff,0x18(%rsp)
    1a57:	ff 
    1a58:	c7 44 24 1c fe ff ff 	movl   $0xfffffffe,0x1c(%rsp)
    1a5f:	ff 
    1a60:	c7 44 24 20 01 00 00 	movl   $0x1,0x20(%rsp)
    1a67:	00 
    1a68:	c7 44 24 24 02 00 00 	movl   $0x2,0x24(%rsp)
    1a6f:	00 
    1a70:	c7 44 24 28 02 00 00 	movl   $0x2,0x28(%rsp)
    1a77:	00 
    1a78:	c7 44 24 2c 01 00 00 	movl   $0x1,0x2c(%rsp)
    1a7f:	00 
    1a80:	c7 44 24 30 ff ff ff 	movl   $0xffffffff,0x30(%rsp)
    1a87:	ff 
    1a88:	c7 44 24 34 fe ff ff 	movl   $0xfffffffe,0x34(%rsp)
    1a8f:	ff 
    1a90:	c7 44 24 38 fe ff ff 	movl   $0xfffffffe,0x38(%rsp)
    1a97:	ff 
    1a98:	c7 44 24 3c ff ff ff 	movl   $0xffffffff,0x3c(%rsp)
    1a9f:	ff 
    1aa0:	c7 44 24 40 ff ff ff 	movl   $0xffffffff,0x40(%rsp)
    1aa7:	ff 
    1aa8:	c7 44 24 44 00 00 00 	movl   $0x0,0x44(%rsp)
    1aaf:	00 
    1ab0:	c7 44 24 48 00 00 00 	movl   $0x0,0x48(%rsp)
    1ab7:	00 
    1ab8:	c7 44 24 4c 01 00 00 	movl   $0x1,0x4c(%rsp)
    1abf:	00 
    1ac0:	c7 44 24 50 01 00 00 	movl   $0x1,0x50(%rsp)
    1ac7:	00 
    1ac8:	c7 44 24 54 00 00 00 	movl   $0x0,0x54(%rsp)
    1acf:	00 
    1ad0:	c7 44 24 58 00 00 00 	movl   $0x0,0x58(%rsp)
    1ad7:	00 
    1ad8:	c7 44 24 5c ff ff ff 	movl   $0xffffffff,0x5c(%rsp)
    1adf:	ff 
    1ae0:	c7 44 24 60 00 00 00 	movl   $0x0,0x60(%rsp)
    1ae7:	00 
    1ae8:	c7 44 24 64 01 00 00 	movl   $0x1,0x64(%rsp)
    1aef:	00 
    1af0:	c7 44 24 68 01 00 00 	movl   $0x1,0x68(%rsp)
    1af7:	00 
    1af8:	c7 44 24 6c 00 00 00 	movl   $0x0,0x6c(%rsp)
    1aff:	00 
    1b00:	c7 44 24 70 00 00 00 	movl   $0x0,0x70(%rsp)
    1b07:	00 
    1b08:	c7 44 24 74 ff ff ff 	movl   $0xffffffff,0x74(%rsp)
    1b0f:	ff 
    1b10:	c7 44 24 78 ff ff ff 	movl   $0xffffffff,0x78(%rsp)
    1b17:	ff 
    1b18:	c7 44 24 7c 00 00 00 	movl   $0x0,0x7c(%rsp)
    1b1f:	00 
    1b20:	83 fe 04             	cmp    $0x4,%esi
    1b23:	75 6b                	jne    1b90 <func7+0x18e>
    1b25:	83 fa 07             	cmp    $0x7,%edx
    1b28:	75 66                	jne    1b90 <func7+0x18e>
    1b2a:	49 63 c9             	movslq %r9d,%rcx
    1b2d:	0f b6 34 0f          	movzbl (%rdi,%rcx,1),%esi
    1b31:	b9 01 00 00 00       	mov    $0x1,%ecx
    1b36:	40 84 f6             	test   %sil,%sil
    1b39:	74 34                	je     1b6f <func7+0x16d>
    1b3b:	b9 00 00 00 00       	mov    $0x0,%ecx
    1b40:	41 83 f9 13          	cmp    $0x13,%r9d
    1b44:	7f 29                	jg     1b6f <func7+0x16d>
    1b46:	41 89 f2             	mov    %esi,%r10d
    1b49:	41 83 e2 07          	and    $0x7,%r10d
    1b4d:	83 e6 07             	and    $0x7,%esi
    1b50:	41 89 c0             	mov    %eax,%r8d
    1b53:	44 03 04 b4          	add    (%rsp,%rsi,4),%r8d
    1b57:	41 89 d3             	mov    %edx,%r11d
    1b5a:	44 03 5c b4 20       	add    0x20(%rsp,%rsi,4),%r11d
    1b5f:	44 89 c6             	mov    %r8d,%esi
    1b62:	44 09 de             	or     %r11d,%esi
    1b65:	b9 00 00 00 00       	mov    $0x0,%ecx
    1b6a:	83 fe 07             	cmp    $0x7,%esi
    1b6d:	76 3f                	jbe    1bae <func7+0x1ac>
    1b6f:	48 8b 84 24 88 00 00 	mov    0x88(%rsp),%rax
    1b76:	00 
    1b77:	64 48 2b 04 25 28 00 	sub    %fs:0x28,%rax
    1b7e:	00 00 
    1b80:	0f 85 9e 00 00 00    	jne    1c24 <func7+0x222>
    1b86:	89 c8                	mov    %ecx,%eax
    1b88:	48 81 c4 98 00 00 00 	add    $0x98,%rsp
    1b8f:	c3                   	ret
    1b90:	b9 00 00 00 00       	mov    $0x0,%ecx
    1b95:	41 83 f9 13          	cmp    $0x13,%r9d
    1b99:	7f d4                	jg     1b6f <func7+0x16d>
    1b9b:	49 63 c9             	movslq %r9d,%rcx
    1b9e:	0f b6 34 0f          	movzbl (%rdi,%rcx,1),%esi
    1ba2:	b9 00 00 00 00       	mov    $0x0,%ecx
    1ba7:	40 84 f6             	test   %sil,%sil
    1baa:	74 c3                	je     1b6f <func7+0x16d>
    1bac:	eb 98                	jmp    1b46 <func7+0x144>
    1bae:	4d 63 d2             	movslq %r10d,%r10
    1bb1:	42 03 44 94 40       	add    0x40(%rsp,%r10,4),%eax
    1bb6:	42 03 54 94 60       	add    0x60(%rsp,%r10,4),%edx
    1bbb:	48 8d 35 ee 45 00 00 	lea    0x45ee(%rip),%rsi        # 61b0 <row0>
    1bc2:	85 c0                	test   %eax,%eax
    1bc4:	7e 0b                	jle    1bd1 <func7+0x1cf>
    1bc6:	48 8b 76 08          	mov    0x8(%rsi),%rsi
    1bca:	83 c1 01             	add    $0x1,%ecx
    1bcd:	39 c8                	cmp    %ecx,%eax
    1bcf:	75 f5                	jne    1bc6 <func7+0x1c4>
    1bd1:	48 63 d2             	movslq %edx,%rdx
    1bd4:	b9 00 00 00 00       	mov    $0x0,%ecx
    1bd9:	80 3c 16 01          	cmpb   $0x1,(%rsi,%rdx,1)
    1bdd:	74 90                	je     1b6f <func7+0x16d>
    1bdf:	48 8d 15 ca 45 00 00 	lea    0x45ca(%rip),%rdx        # 61b0 <row0>
    1be6:	45 85 c0             	test   %r8d,%r8d
    1be9:	7e 11                	jle    1bfc <func7+0x1fa>
    1beb:	b8 00 00 00 00       	mov    $0x0,%eax
    1bf0:	48 8b 52 08          	mov    0x8(%rdx),%rdx
    1bf4:	83 c0 01             	add    $0x1,%eax
    1bf7:	41 39 c0             	cmp    %eax,%r8d
    1bfa:	75 f4                	jne    1bf0 <func7+0x1ee>
    1bfc:	49 63 c3             	movslq %r11d,%rax
    1bff:	b9 00 00 00 00       	mov    $0x0,%ecx
    1c04:	80 3c 02 01          	cmpb   $0x1,(%rdx,%rax,1)
    1c08:	0f 84 61 ff ff ff    	je     1b6f <func7+0x16d>
    1c0e:	41 8d 49 01          	lea    0x1(%r9),%ecx
    1c12:	44 89 da             	mov    %r11d,%edx
    1c15:	44 89 c6             	mov    %r8d,%esi
    1c18:	e8 e5 fd ff ff       	call   1a02 <func7>
    1c1d:	89 c1                	mov    %eax,%ecx
    1c1f:	e9 4b ff ff ff       	jmp    1b6f <func7+0x16d>
    1c24:	e8 77 f4 ff ff       	call   10a0 <__stack_chk_fail@plt>

0000000000001c29 <secret_phase>:
    1c29:	53                   	push   %rbx
    1c2a:	48 8d 3d 04 16 00 00 	lea    0x1604(%rip),%rdi        # 3235 <_IO_stdin_used+0x235>
    1c31:	e8 3a f4 ff ff       	call   1070 <puts@plt>
    1c36:	e8 48 04 00 00       	call   2083 <read_line>
    1c3b:	48 89 c3             	mov    %rax,%rbx
    1c3e:	48 89 c7             	mov    %rax,%rdi
    1c41:	e8 ba 00 00 00       	call   1d00 <string_length>
    1c46:	83 f8 14             	cmp    $0x14,%eax
    1c49:	7f 2e                	jg     1c79 <secret_phase+0x50>
    1c4b:	b9 00 00 00 00       	mov    $0x0,%ecx
    1c50:	ba 00 00 00 00       	mov    $0x0,%edx
    1c55:	be 00 00 00 00       	mov    $0x0,%esi
    1c5a:	48 89 df             	mov    %rbx,%rdi
    1c5d:	e8 a0 fd ff ff       	call   1a02 <func7>
    1c62:	85 c0                	test   %eax,%eax
    1c64:	74 1a                	je     1c80 <secret_phase+0x57>
    1c66:	48 8d 3d 83 15 00 00 	lea    0x1583(%rip),%rdi        # 31f0 <_IO_stdin_used+0x1f0>
    1c6d:	e8 fe f3 ff ff       	call   1070 <puts@plt>
    1c72:	e8 46 05 00 00       	call   21bd <phase_defused>
    1c77:	5b                   	pop    %rbx
    1c78:	c3                   	ret
    1c79:	e8 04 03 00 00       	call   1f82 <explode_bomb>
    1c7e:	eb cb                	jmp    1c4b <secret_phase+0x22>
    1c80:	e8 fd 02 00 00       	call   1f82 <explode_bomb>
    1c85:	eb df                	jmp    1c66 <secret_phase+0x3d>

0000000000001c87 <sig_handler>:
    1c87:	48 83 ec 08          	sub    $0x8,%rsp
    1c8b:	48 8d 3d 2e 16 00 00 	lea    0x162e(%rip),%rdi        # 32c0 <array.0+0x40>
    1c92:	e8 d9 f3 ff ff       	call   1070 <puts@plt>
    1c97:	bf 03 00 00 00       	mov    $0x3,%edi
    1c9c:	e8 0f f5 ff ff       	call   11b0 <sleep@plt>
    1ca1:	48 8d 3d 91 17 00 00 	lea    0x1791(%rip),%rdi        # 3439 <array.0+0x1b9>
    1ca8:	b8 00 00 00 00       	mov    $0x0,%eax
    1cad:	e8 fe f3 ff ff       	call   10b0 <printf@plt>
    1cb2:	48 8b 3d c7 49 00 00 	mov    0x49c7(%rip),%rdi        # 6680 <stdout@GLIBC_2.2.5>
    1cb9:	e8 82 f4 ff ff       	call   1140 <fflush@plt>
    1cbe:	bf 01 00 00 00       	mov    $0x1,%edi
    1cc3:	e8 e8 f4 ff ff       	call   11b0 <sleep@plt>
    1cc8:	48 8d 3d 72 17 00 00 	lea    0x1772(%rip),%rdi        # 3441 <array.0+0x1c1>
    1ccf:	e8 9c f3 ff ff       	call   1070 <puts@plt>
    1cd4:	bf 10 00 00 00       	mov    $0x10,%edi
    1cd9:	e8 b2 f4 ff ff       	call   1190 <exit@plt>

0000000000001cde <invalid_phase>:
    1cde:	48 83 ec 08          	sub    $0x8,%rsp
    1ce2:	48 89 fe             	mov    %rdi,%rsi
    1ce5:	48 8d 3d 5f 17 00 00 	lea    0x175f(%rip),%rdi        # 344b <array.0+0x1cb>
    1cec:	b8 00 00 00 00       	mov    $0x0,%eax
    1cf1:	e8 ba f3 ff ff       	call   10b0 <printf@plt>
    1cf6:	bf 08 00 00 00       	mov    $0x8,%edi
    1cfb:	e8 90 f4 ff ff       	call   1190 <exit@plt>

0000000000001d00 <string_length>:
    1d00:	80 3f 00             	cmpb   $0x0,(%rdi)
    1d03:	74 12                	je     1d17 <string_length+0x17>
    1d05:	b8 00 00 00 00       	mov    $0x0,%eax
    1d0a:	48 83 c7 01          	add    $0x1,%rdi
    1d0e:	83 c0 01             	add    $0x1,%eax
    1d11:	80 3f 00             	cmpb   $0x0,(%rdi)
    1d14:	75 f4                	jne    1d0a <string_length+0xa>
    1d16:	c3                   	ret
    1d17:	b8 00 00 00 00       	mov    $0x0,%eax
    1d1c:	c3                   	ret

0000000000001d1d <strings_not_equal>:
    1d1d:	41 54                	push   %r12
    1d1f:	55                   	push   %rbp
    1d20:	53                   	push   %rbx
    1d21:	48 89 fb             	mov    %rdi,%rbx
    1d24:	48 89 f5             	mov    %rsi,%rbp
    1d27:	e8 d4 ff ff ff       	call   1d00 <string_length>
    1d2c:	41 89 c4             	mov    %eax,%r12d
    1d2f:	48 89 ef             	mov    %rbp,%rdi
    1d32:	e8 c9 ff ff ff       	call   1d00 <string_length>
    1d37:	89 c2                	mov    %eax,%edx
    1d39:	b8 01 00 00 00       	mov    $0x1,%eax
    1d3e:	41 39 d4             	cmp    %edx,%r12d
    1d41:	75 31                	jne    1d74 <strings_not_equal+0x57>
    1d43:	0f b6 13             	movzbl (%rbx),%edx
    1d46:	84 d2                	test   %dl,%dl
    1d48:	74 1e                	je     1d68 <strings_not_equal+0x4b>
    1d4a:	b8 00 00 00 00       	mov    $0x0,%eax
    1d4f:	38 54 05 00          	cmp    %dl,0x0(%rbp,%rax,1)
    1d53:	75 1a                	jne    1d6f <strings_not_equal+0x52>
    1d55:	48 83 c0 01          	add    $0x1,%rax
    1d59:	0f b6 14 03          	movzbl (%rbx,%rax,1),%edx
    1d5d:	84 d2                	test   %dl,%dl
    1d5f:	75 ee                	jne    1d4f <strings_not_equal+0x32>
    1d61:	b8 00 00 00 00       	mov    $0x0,%eax
    1d66:	eb 0c                	jmp    1d74 <strings_not_equal+0x57>
    1d68:	b8 00 00 00 00       	mov    $0x0,%eax
    1d6d:	eb 05                	jmp    1d74 <strings_not_equal+0x57>
    1d6f:	b8 01 00 00 00       	mov    $0x1,%eax
    1d74:	5b                   	pop    %rbx
    1d75:	5d                   	pop    %rbp
    1d76:	41 5c                	pop    %r12
    1d78:	c3                   	ret

0000000000001d79 <initialize_bomb>:
    1d79:	55                   	push   %rbp
    1d7a:	53                   	push   %rbx
    1d7b:	48 83 ec 58          	sub    $0x58,%rsp
    1d7f:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1d86:	00 00 
    1d88:	48 89 44 24 48       	mov    %rax,0x48(%rsp)
    1d8d:	31 c0                	xor    %eax,%eax
    1d8f:	48 8d 35 f1 fe ff ff 	lea    -0x10f(%rip),%rsi        # 1c87 <sig_handler>
    1d96:	bf 02 00 00 00       	mov    $0x2,%edi
    1d9b:	e8 70 f3 ff ff       	call   1110 <signal@plt>
    1da0:	48 8b 3d d9 44 00 00 	mov    0x44d9(%rip),%rdi        # 6280 <host_table>
    1da7:	48 85 ff             	test   %rdi,%rdi
    1daa:	74 23                	je     1dcf <initialize_bomb+0x56>
    1dac:	48 8d 1d d5 44 00 00 	lea    0x44d5(%rip),%rbx        # 6288 <host_table+0x8>
    1db3:	48 89 e5             	mov    %rsp,%rbp
    1db6:	48 89 ee             	mov    %rbp,%rsi
    1db9:	e8 82 f2 ff ff       	call   1040 <strcasecmp@plt>
    1dbe:	85 c0                	test   %eax,%eax
    1dc0:	74 0d                	je     1dcf <initialize_bomb+0x56>
    1dc2:	48 83 c3 08          	add    $0x8,%rbx
    1dc6:	48 8b 7b f8          	mov    -0x8(%rbx),%rdi
    1dca:	48 85 ff             	test   %rdi,%rdi
    1dcd:	75 e7                	jne    1db6 <initialize_bomb+0x3d>
    1dcf:	48 8d 3d 86 16 00 00 	lea    0x1686(%rip),%rdi        # 345c <array.0+0x1dc>
    1dd6:	e8 95 f2 ff ff       	call   1070 <puts@plt>
    1ddb:	48 8d 3d 86 16 00 00 	lea    0x1686(%rip),%rdi        # 3468 <array.0+0x1e8>
    1de2:	e8 89 f2 ff ff       	call   1070 <puts@plt>
    1de7:	48 8d 3d 86 16 00 00 	lea    0x1686(%rip),%rdi        # 3474 <array.0+0x1f4>
    1dee:	e8 7d f2 ff ff       	call   1070 <puts@plt>
    1df3:	48 8d 3d 86 16 00 00 	lea    0x1686(%rip),%rdi        # 3480 <array.0+0x200>
    1dfa:	e8 71 f2 ff ff       	call   1070 <puts@plt>
    1dff:	48 8d 3d 86 16 00 00 	lea    0x1686(%rip),%rdi        # 348c <array.0+0x20c>
    1e06:	e8 65 f2 ff ff       	call   1070 <puts@plt>
    1e0b:	48 8b 44 24 48       	mov    0x48(%rsp),%rax
    1e10:	64 48 2b 04 25 28 00 	sub    %fs:0x28,%rax
    1e17:	00 00 
    1e19:	75 07                	jne    1e22 <initialize_bomb+0xa9>
    1e1b:	48 83 c4 58          	add    $0x58,%rsp
    1e1f:	5b                   	pop    %rbx
    1e20:	5d                   	pop    %rbp
    1e21:	c3                   	ret
    1e22:	e8 79 f2 ff ff       	call   10a0 <__stack_chk_fail@plt>

0000000000001e27 <initialize_bomb_solve>:
    1e27:	c3                   	ret

0000000000001e28 <blank_line>:
    1e28:	55                   	push   %rbp
    1e29:	53                   	push   %rbx
    1e2a:	48 83 ec 08          	sub    $0x8,%rsp
    1e2e:	48 89 fd             	mov    %rdi,%rbp
    1e31:	0f b6 5d 00          	movzbl 0x0(%rbp),%ebx
    1e35:	84 db                	test   %bl,%bl
    1e37:	74 1e                	je     1e57 <blank_line+0x2f>
    1e39:	e8 82 f3 ff ff       	call   11c0 <__ctype_b_loc@plt>
    1e3e:	48 83 c5 01          	add    $0x1,%rbp
    1e42:	48 0f be db          	movsbq %bl,%rbx
    1e46:	48 8b 00             	mov    (%rax),%rax
    1e49:	f6 44 58 01 20       	testb  $0x20,0x1(%rax,%rbx,2)
    1e4e:	75 e1                	jne    1e31 <blank_line+0x9>
    1e50:	b8 00 00 00 00       	mov    $0x0,%eax
    1e55:	eb 05                	jmp    1e5c <blank_line+0x34>
    1e57:	b8 01 00 00 00       	mov    $0x1,%eax
    1e5c:	48 83 c4 08          	add    $0x8,%rsp
    1e60:	5b                   	pop    %rbx
    1e61:	5d                   	pop    %rbp
    1e62:	c3                   	ret

0000000000001e63 <skip>:
    1e63:	55                   	push   %rbp
    1e64:	53                   	push   %rbx
    1e65:	48 83 ec 08          	sub    $0x8,%rsp
    1e69:	48 8d 2d d0 48 00 00 	lea    0x48d0(%rip),%rbp        # 6740 <input_strings>
    1e70:	48 63 15 c1 48 00 00 	movslq 0x48c1(%rip),%rdx        # 6738 <num_input_strings>
    1e77:	48 89 d0             	mov    %rdx,%rax
    1e7a:	48 c1 e0 04          	shl    $0x4,%rax
    1e7e:	48 29 d0             	sub    %rdx,%rax
    1e81:	48 8d 7c c5 00       	lea    0x0(%rbp,%rax,8),%rdi
    1e86:	48 8b 15 23 48 00 00 	mov    0x4823(%rip),%rdx        # 66b0 <infile>
    1e8d:	be 78 00 00 00       	mov    $0x78,%esi
    1e92:	e8 59 f2 ff ff       	call   10f0 <fgets@plt>
    1e97:	48 89 c3             	mov    %rax,%rbx
    1e9a:	48 85 c0             	test   %rax,%rax
    1e9d:	74 0c                	je     1eab <skip+0x48>
    1e9f:	48 89 c7             	mov    %rax,%rdi
    1ea2:	e8 81 ff ff ff       	call   1e28 <blank_line>
    1ea7:	85 c0                	test   %eax,%eax
    1ea9:	75 c5                	jne    1e70 <skip+0xd>
    1eab:	48 89 d8             	mov    %rbx,%rax
    1eae:	48 83 c4 08          	add    $0x8,%rsp
    1eb2:	5b                   	pop    %rbx
    1eb3:	5d                   	pop    %rbp
    1eb4:	c3                   	ret

0000000000001eb5 <send_msg>:
    1eb5:	53                   	push   %rbx
    1eb6:	48 81 ec 10 40 00 00 	sub    $0x4010,%rsp
    1ebd:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1ec4:	00 00 
    1ec6:	48 89 84 24 08 40 00 	mov    %rax,0x4008(%rsp)
    1ecd:	00 
    1ece:	31 c0                	xor    %eax,%eax
    1ed0:	44 8b 05 61 48 00 00 	mov    0x4861(%rip),%r8d        # 6738 <num_input_strings>
    1ed7:	41 8d 40 ff          	lea    -0x1(%r8),%eax
    1edb:	48 98                	cltq
    1edd:	48 89 c2             	mov    %rax,%rdx
    1ee0:	48 c1 e2 04          	shl    $0x4,%rdx
    1ee4:	48 29 c2             	sub    %rax,%rdx
    1ee7:	85 ff                	test   %edi,%edi
    1ee9:	48 8d 0d a8 15 00 00 	lea    0x15a8(%rip),%rcx        # 3498 <array.0+0x218>
    1ef0:	48 8d 05 a9 15 00 00 	lea    0x15a9(%rip),%rax        # 34a0 <array.0+0x220>
    1ef7:	48 0f 44 c8          	cmove  %rax,%rcx
    1efb:	48 89 e3             	mov    %rsp,%rbx
    1efe:	48 8d 05 3b 48 00 00 	lea    0x483b(%rip),%rax        # 6740 <input_strings>
    1f05:	4c 8d 0c d0          	lea    (%rax,%rdx,8),%r9
    1f09:	8b 15 95 42 00 00    	mov    0x4295(%rip),%edx        # 61a4 <bomb_id>
    1f0f:	48 8d 35 93 15 00 00 	lea    0x1593(%rip),%rsi        # 34a9 <array.0+0x229>
    1f16:	48 89 df             	mov    %rbx,%rdi
    1f19:	b8 00 00 00 00       	mov    $0x0,%eax
    1f1e:	e8 5d f2 ff ff       	call   1180 <sprintf@plt>
    1f23:	4c 8d 84 24 00 20 00 	lea    0x2000(%rsp),%r8
    1f2a:	00 
    1f2b:	b9 00 00 00 00       	mov    $0x0,%ecx
    1f30:	48 89 da             	mov    %rbx,%rdx
    1f33:	48 8d 35 46 42 00 00 	lea    0x4246(%rip),%rsi        # 6180 <user_password>
    1f3a:	48 8d 3d 57 42 00 00 	lea    0x4257(%rip),%rdi        # 6198 <userid>
    1f41:	e8 ac 0e 00 00       	call   2df2 <driver_post>
    1f46:	85 c0                	test   %eax,%eax
    1f48:	78 1c                	js     1f66 <send_msg+0xb1>
    1f4a:	48 8b 84 24 08 40 00 	mov    0x4008(%rsp),%rax
    1f51:	00 
    1f52:	64 48 2b 04 25 28 00 	sub    %fs:0x28,%rax
    1f59:	00 00 
    1f5b:	75 20                	jne    1f7d <send_msg+0xc8>
    1f5d:	48 81 c4 10 40 00 00 	add    $0x4010,%rsp
    1f64:	5b                   	pop    %rbx
    1f65:	c3                   	ret
    1f66:	48 8d bc 24 00 20 00 	lea    0x2000(%rsp),%rdi
    1f6d:	00 
    1f6e:	e8 fd f0 ff ff       	call   1070 <puts@plt>
    1f73:	bf 00 00 00 00       	mov    $0x0,%edi
    1f78:	e8 13 f2 ff ff       	call   1190 <exit@plt>
    1f7d:	e8 1e f1 ff ff       	call   10a0 <__stack_chk_fail@plt>

0000000000001f82 <explode_bomb>:
    1f82:	48 83 ec 08          	sub    $0x8,%rsp
    1f86:	48 8d 3d 6b 13 00 00 	lea    0x136b(%rip),%rdi        # 32f8 <array.0+0x78>
    1f8d:	e8 de f0 ff ff       	call   1070 <puts@plt>
    1f92:	48 8d 3d 1c 15 00 00 	lea    0x151c(%rip),%rdi        # 34b5 <array.0+0x235>
    1f99:	e8 d2 f0 ff ff       	call   1070 <puts@plt>
    1f9e:	48 8d 3d 2d 15 00 00 	lea    0x152d(%rip),%rdi        # 34d2 <array.0+0x252>
    1fa5:	e8 c6 f0 ff ff       	call   1070 <puts@plt>
    1faa:	48 8d 3d 3e 15 00 00 	lea    0x153e(%rip),%rdi        # 34ef <array.0+0x26f>
    1fb1:	e8 ba f0 ff ff       	call   1070 <puts@plt>
    1fb6:	48 8d 3d 4f 15 00 00 	lea    0x154f(%rip),%rdi        # 350c <array.0+0x28c>
    1fbd:	e8 ae f0 ff ff       	call   1070 <puts@plt>
    1fc2:	48 8d 3d 60 15 00 00 	lea    0x1560(%rip),%rdi        # 3529 <array.0+0x2a9>
    1fc9:	e8 a2 f0 ff ff       	call   1070 <puts@plt>
    1fce:	48 8d 3d 70 15 00 00 	lea    0x1570(%rip),%rdi        # 3545 <array.0+0x2c5>
    1fd5:	e8 96 f0 ff ff       	call   1070 <puts@plt>
    1fda:	48 8d 3d 81 15 00 00 	lea    0x1581(%rip),%rdi        # 3562 <array.0+0x2e2>
    1fe1:	e8 8a f0 ff ff       	call   1070 <puts@plt>
    1fe6:	48 8d 3d 92 15 00 00 	lea    0x1592(%rip),%rdi        # 357f <array.0+0x2ff>
    1fed:	e8 7e f0 ff ff       	call   1070 <puts@plt>
    1ff2:	48 8d 3d a3 15 00 00 	lea    0x15a3(%rip),%rdi        # 359c <array.0+0x31c>
    1ff9:	e8 72 f0 ff ff       	call   1070 <puts@plt>
    1ffe:	48 8d 3d b4 15 00 00 	lea    0x15b4(%rip),%rdi        # 35b9 <array.0+0x339>
    2005:	e8 66 f0 ff ff       	call   1070 <puts@plt>
    200a:	48 8d 3d c5 15 00 00 	lea    0x15c5(%rip),%rdi        # 35d6 <array.0+0x356>
    2011:	e8 5a f0 ff ff       	call   1070 <puts@plt>
    2016:	48 8d 3d d6 15 00 00 	lea    0x15d6(%rip),%rdi        # 35f3 <array.0+0x373>
    201d:	e8 4e f0 ff ff       	call   1070 <puts@plt>
    2022:	bf 00 00 00 00       	mov    $0x0,%edi
    2027:	e8 89 fe ff ff       	call   1eb5 <send_msg>
    202c:	48 8d 3d 0d 13 00 00 	lea    0x130d(%rip),%rdi        # 3340 <array.0+0xc0>
    2033:	e8 38 f0 ff ff       	call   1070 <puts@plt>
    2038:	bf 08 00 00 00       	mov    $0x8,%edi
    203d:	e8 4e f1 ff ff       	call   1190 <exit@plt>

0000000000002042 <read_six_numbers>:
    2042:	48 83 ec 08          	sub    $0x8,%rsp
    2046:	48 89 f2             	mov    %rsi,%rdx
    2049:	48 8d 4e 04          	lea    0x4(%rsi),%rcx
    204d:	48 8d 46 14          	lea    0x14(%rsi),%rax
    2051:	50                   	push   %rax
    2052:	48 8d 46 10          	lea    0x10(%rsi),%rax
    2056:	50                   	push   %rax
    2057:	4c 8d 4e 0c          	lea    0xc(%rsi),%r9
    205b:	4c 8d 46 08          	lea    0x8(%rsi),%r8
    205f:	48 8d 35 a5 15 00 00 	lea    0x15a5(%rip),%rsi        # 360b <array.0+0x38b>
    2066:	b8 00 00 00 00       	mov    $0x0,%eax
    206b:	e8 e0 f0 ff ff       	call   1150 <__isoc99_sscanf@plt>
    2070:	48 83 c4 10          	add    $0x10,%rsp
    2074:	83 f8 05             	cmp    $0x5,%eax
    2077:	7e 05                	jle    207e <read_six_numbers+0x3c>
    2079:	48 83 c4 08          	add    $0x8,%rsp
    207d:	c3                   	ret
    207e:	e8 ff fe ff ff       	call   1f82 <explode_bomb>

0000000000002083 <read_line>:
    2083:	55                   	push   %rbp
    2084:	53                   	push   %rbx
    2085:	48 83 ec 08          	sub    $0x8,%rsp
    2089:	b8 00 00 00 00       	mov    $0x0,%eax
    208e:	e8 d0 fd ff ff       	call   1e63 <skip>
    2093:	48 85 c0             	test   %rax,%rax
    2096:	74 63                	je     20fb <read_line+0x78>
    2098:	8b 1d 9a 46 00 00    	mov    0x469a(%rip),%ebx        # 6738 <num_input_strings>
    209e:	48 63 d3             	movslq %ebx,%rdx
    20a1:	48 89 d0             	mov    %rdx,%rax
    20a4:	48 c1 e0 04          	shl    $0x4,%rax
    20a8:	48 29 d0             	sub    %rdx,%rax
    20ab:	48 8d 15 8e 46 00 00 	lea    0x468e(%rip),%rdx        # 6740 <input_strings>
    20b2:	48 8d 2c c2          	lea    (%rdx,%rax,8),%rbp
    20b6:	48 89 ef             	mov    %rbp,%rdi
    20b9:	e8 d2 ef ff ff       	call   1090 <strlen@plt>
    20be:	83 f8 76             	cmp    $0x76,%eax
    20c1:	0f 8f ac 00 00 00    	jg     2173 <read_line+0xf0>
    20c7:	83 e8 01             	sub    $0x1,%eax
    20ca:	48 98                	cltq
    20cc:	48 63 cb             	movslq %ebx,%rcx
    20cf:	48 89 ca             	mov    %rcx,%rdx
    20d2:	48 c1 e2 04          	shl    $0x4,%rdx
    20d6:	48 29 ca             	sub    %rcx,%rdx
    20d9:	48 8d 0d 60 46 00 00 	lea    0x4660(%rip),%rcx        # 6740 <input_strings>
    20e0:	48 8d 14 d1          	lea    (%rcx,%rdx,8),%rdx
    20e4:	c6 04 02 00          	movb   $0x0,(%rdx,%rax,1)
    20e8:	83 c3 01             	add    $0x1,%ebx
    20eb:	89 1d 47 46 00 00    	mov    %ebx,0x4647(%rip)        # 6738 <num_input_strings>
    20f1:	48 89 e8             	mov    %rbp,%rax
    20f4:	48 83 c4 08          	add    $0x8,%rsp
    20f8:	5b                   	pop    %rbx
    20f9:	5d                   	pop    %rbp
    20fa:	c3                   	ret
    20fb:	48 8b 05 8e 45 00 00 	mov    0x458e(%rip),%rax        # 6690 <stdin@GLIBC_2.2.5>
    2102:	48 39 05 a7 45 00 00 	cmp    %rax,0x45a7(%rip)        # 66b0 <infile>
    2109:	74 1b                	je     2126 <read_line+0xa3>
    210b:	48 8d 3d 29 15 00 00 	lea    0x1529(%rip),%rdi        # 363b <array.0+0x3bb>
    2112:	e8 19 ef ff ff       	call   1030 <getenv@plt>
    2117:	48 85 c0             	test   %rax,%rax
    211a:	74 20                	je     213c <read_line+0xb9>
    211c:	bf 00 00 00 00       	mov    $0x0,%edi
    2121:	e8 6a f0 ff ff       	call   1190 <exit@plt>
    2126:	48 8d 3d f0 14 00 00 	lea    0x14f0(%rip),%rdi        # 361d <array.0+0x39d>
    212d:	e8 3e ef ff ff       	call   1070 <puts@plt>
    2132:	bf 08 00 00 00       	mov    $0x8,%edi
    2137:	e8 54 f0 ff ff       	call   1190 <exit@plt>
    213c:	48 8b 05 4d 45 00 00 	mov    0x454d(%rip),%rax        # 6690 <stdin@GLIBC_2.2.5>
    2143:	48 89 05 66 45 00 00 	mov    %rax,0x4566(%rip)        # 66b0 <infile>
    214a:	b8 00 00 00 00       	mov    $0x0,%eax
    214f:	e8 0f fd ff ff       	call   1e63 <skip>
    2154:	48 85 c0             	test   %rax,%rax
    2157:	0f 85 3b ff ff ff    	jne    2098 <read_line+0x15>
    215d:	48 8d 3d b9 14 00 00 	lea    0x14b9(%rip),%rdi        # 361d <array.0+0x39d>
    2164:	e8 07 ef ff ff       	call   1070 <puts@plt>
    2169:	bf 00 00 00 00       	mov    $0x0,%edi
    216e:	e8 1d f0 ff ff       	call   1190 <exit@plt>
    2173:	48 8d 3d cc 14 00 00 	lea    0x14cc(%rip),%rdi        # 3646 <array.0+0x3c6>
    217a:	e8 f1 ee ff ff       	call   1070 <puts@plt>
    217f:	8b 05 b3 45 00 00    	mov    0x45b3(%rip),%eax        # 6738 <num_input_strings>
    2185:	8d 50 01             	lea    0x1(%rax),%edx
    2188:	89 15 aa 45 00 00    	mov    %edx,0x45aa(%rip)        # 6738 <num_input_strings>
    218e:	48 98                	cltq
    2190:	48 6b c0 78          	imul   $0x78,%rax,%rax
    2194:	48 8d 15 a5 45 00 00 	lea    0x45a5(%rip),%rdx        # 6740 <input_strings>
    219b:	48 be 2a 2a 2a 74 72 	movabs $0x636e7572742a2a2a,%rsi
    21a2:	75 6e 63 
    21a5:	48 bf 61 74 65 64 2a 	movabs $0x2a2a2a64657461,%rdi
    21ac:	2a 2a 00 
    21af:	48 89 34 02          	mov    %rsi,(%rdx,%rax,1)
    21b3:	48 89 7c 02 08       	mov    %rdi,0x8(%rdx,%rax,1)
    21b8:	e8 c5 fd ff ff       	call   1f82 <explode_bomb>

00000000000021bd <phase_defused>:
    21bd:	48 83 ec 08          	sub    $0x8,%rsp
    21c1:	bf 01 00 00 00       	mov    $0x1,%edi
    21c6:	e8 ea fc ff ff       	call   1eb5 <send_msg>
    21cb:	83 3d 66 45 00 00 06 	cmpl   $0x6,0x4566(%rip)        # 6738 <num_input_strings>
    21d2:	74 05                	je     21d9 <phase_defused+0x1c>
    21d4:	48 83 c4 08          	add    $0x8,%rsp
    21d8:	c3                   	ret
    21d9:	0f b6 0d b8 47 00 00 	movzbl 0x47b8(%rip),%ecx        # 6998 <input_strings+0x258>
    21e0:	84 c9                	test   %cl,%cl
    21e2:	74 34                	je     2218 <phase_defused+0x5b>
    21e4:	b8 01 00 00 00       	mov    $0x1,%eax
    21e9:	ba 00 00 00 00       	mov    $0x0,%edx
    21ee:	48 8d 3d a3 47 00 00 	lea    0x47a3(%rip),%rdi        # 6998 <input_strings+0x258>
    21f5:	80 f9 20             	cmp    $0x20,%cl
    21f8:	0f 94 c1             	sete   %cl
    21fb:	0f b6 c9             	movzbl %cl,%ecx
    21fe:	01 ca                	add    %ecx,%edx
    2200:	89 c6                	mov    %eax,%esi
    2202:	0f b6 0c 07          	movzbl (%rdi,%rax,1),%ecx
    2206:	48 83 c0 01          	add    $0x1,%rax
    220a:	83 fa 05             	cmp    $0x5,%edx
    220d:	7f 04                	jg     2213 <phase_defused+0x56>
    220f:	84 c9                	test   %cl,%cl
    2211:	75 e2                	jne    21f5 <phase_defused+0x38>
    2213:	83 fa 06             	cmp    $0x6,%edx
    2216:	74 1a                	je     2232 <phase_defused+0x75>
    2218:	48 8d 3d a9 11 00 00 	lea    0x11a9(%rip),%rdi        # 33c8 <array.0+0x148>
    221f:	e8 4c ee ff ff       	call   1070 <puts@plt>
    2224:	48 8d 3d cd 11 00 00 	lea    0x11cd(%rip),%rdi        # 33f8 <array.0+0x178>
    222b:	e8 40 ee ff ff       	call   1070 <puts@plt>
    2230:	eb a2                	jmp    21d4 <phase_defused+0x17>
    2232:	48 63 f6             	movslq %esi,%rsi
    2235:	48 8d 05 5c 47 00 00 	lea    0x475c(%rip),%rax        # 6998 <input_strings+0x258>
    223c:	48 8d 3c 06          	lea    (%rsi,%rax,1),%rdi
    2240:	48 8d 35 1a 14 00 00 	lea    0x141a(%rip),%rsi        # 3661 <array.0+0x3e1>
    2247:	e8 d1 fa ff ff       	call   1d1d <strings_not_equal>
    224c:	85 c0                	test   %eax,%eax
    224e:	75 c8                	jne    2218 <phase_defused+0x5b>
    2250:	48 8d 3d 11 11 00 00 	lea    0x1111(%rip),%rdi        # 3368 <array.0+0xe8>
    2257:	e8 14 ee ff ff       	call   1070 <puts@plt>
    225c:	48 8d 3d 2d 11 00 00 	lea    0x112d(%rip),%rdi        # 3390 <array.0+0x110>
    2263:	e8 08 ee ff ff       	call   1070 <puts@plt>
    2268:	b8 00 00 00 00       	mov    $0x0,%eax
    226d:	e8 b7 f9 ff ff       	call   1c29 <secret_phase>
    2272:	eb a4                	jmp    2218 <phase_defused+0x5b>

0000000000002274 <sigalrm_handler>:
    2274:	48 83 ec 08          	sub    $0x8,%rsp
    2278:	ba 00 00 00 00       	mov    $0x0,%edx
    227d:	48 8d 35 f4 13 00 00 	lea    0x13f4(%rip),%rsi        # 3678 <array.0+0x3f8>
    2284:	48 8b 3d 15 44 00 00 	mov    0x4415(%rip),%rdi        # 66a0 <stderr@GLIBC_2.2.5>
    228b:	b8 00 00 00 00       	mov    $0x0,%eax
    2290:	e8 9b ee ff ff       	call   1130 <fprintf@plt>
    2295:	bf 01 00 00 00       	mov    $0x1,%edi
    229a:	e8 f1 ee ff ff       	call   1190 <exit@plt>

000000000000229f <rio_writen>:
    229f:	41 56                	push   %r14
    22a1:	41 55                	push   %r13
    22a3:	41 54                	push   %r12
    22a5:	55                   	push   %rbp
    22a6:	53                   	push   %rbx
    22a7:	49 89 d5             	mov    %rdx,%r13
    22aa:	48 85 d2             	test   %rdx,%rdx
    22ad:	74 3b                	je     22ea <rio_writen+0x4b>
    22af:	41 89 fc             	mov    %edi,%r12d
    22b2:	48 89 f5             	mov    %rsi,%rbp
    22b5:	48 89 d3             	mov    %rdx,%rbx
    22b8:	41 be 00 00 00 00    	mov    $0x0,%r14d
    22be:	eb 08                	jmp    22c8 <rio_writen+0x29>
    22c0:	48 01 c5             	add    %rax,%rbp
    22c3:	48 29 c3             	sub    %rax,%rbx
    22c6:	74 22                	je     22ea <rio_writen+0x4b>
    22c8:	48 89 da             	mov    %rbx,%rdx
    22cb:	48 89 ee             	mov    %rbp,%rsi
    22ce:	44 89 e7             	mov    %r12d,%edi
    22d1:	e8 aa ed ff ff       	call   1080 <write@plt>
    22d6:	48 85 c0             	test   %rax,%rax
    22d9:	7f e5                	jg     22c0 <rio_writen+0x21>
    22db:	e8 70 ed ff ff       	call   1050 <__errno_location@plt>
    22e0:	83 38 04             	cmpl   $0x4,(%rax)
    22e3:	75 11                	jne    22f6 <rio_writen+0x57>
    22e5:	4c 89 f0             	mov    %r14,%rax
    22e8:	eb d6                	jmp    22c0 <rio_writen+0x21>
    22ea:	4c 89 e8             	mov    %r13,%rax
    22ed:	5b                   	pop    %rbx
    22ee:	5d                   	pop    %rbp
    22ef:	41 5c                	pop    %r12
    22f1:	41 5d                	pop    %r13
    22f3:	41 5e                	pop    %r14
    22f5:	c3                   	ret
    22f6:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
    22fd:	eb ee                	jmp    22ed <rio_writen+0x4e>

00000000000022ff <rio_readlineb>:
    22ff:	41 56                	push   %r14
    2301:	41 55                	push   %r13
    2303:	41 54                	push   %r12
    2305:	55                   	push   %rbp
    2306:	53                   	push   %rbx
    2307:	49 89 f4             	mov    %rsi,%r12
    230a:	48 83 fa 01          	cmp    $0x1,%rdx
    230e:	0f 86 92 00 00 00    	jbe    23a6 <rio_readlineb+0xa7>
    2314:	48 89 fb             	mov    %rdi,%rbx
    2317:	4c 8d 74 16 ff       	lea    -0x1(%rsi,%rdx,1),%r14
    231c:	41 bd 01 00 00 00    	mov    $0x1,%r13d
    2322:	48 8d 6f 10          	lea    0x10(%rdi),%rbp
    2326:	eb 56                	jmp    237e <rio_readlineb+0x7f>
    2328:	e8 23 ed ff ff       	call   1050 <__errno_location@plt>
    232d:	83 38 04             	cmpl   $0x4,(%rax)
    2330:	75 55                	jne    2387 <rio_readlineb+0x88>
    2332:	ba 00 20 00 00       	mov    $0x2000,%edx
    2337:	48 89 ee             	mov    %rbp,%rsi
    233a:	8b 3b                	mov    (%rbx),%edi
    233c:	e8 9f ed ff ff       	call   10e0 <read@plt>
    2341:	89 c2                	mov    %eax,%edx
    2343:	89 43 04             	mov    %eax,0x4(%rbx)
    2346:	85 c0                	test   %eax,%eax
    2348:	78 de                	js     2328 <rio_readlineb+0x29>
    234a:	85 c0                	test   %eax,%eax
    234c:	74 42                	je     2390 <rio_readlineb+0x91>
    234e:	48 89 6b 08          	mov    %rbp,0x8(%rbx)
    2352:	48 8b 43 08          	mov    0x8(%rbx),%rax
    2356:	0f b6 08             	movzbl (%rax),%ecx
    2359:	48 83 c0 01          	add    $0x1,%rax
    235d:	48 89 43 08          	mov    %rax,0x8(%rbx)
    2361:	83 ea 01             	sub    $0x1,%edx
    2364:	89 53 04             	mov    %edx,0x4(%rbx)
    2367:	49 83 c4 01          	add    $0x1,%r12
    236b:	41 88 4c 24 ff       	mov    %cl,-0x1(%r12)
    2370:	80 f9 0a             	cmp    $0xa,%cl
    2373:	74 3c                	je     23b1 <rio_readlineb+0xb2>
    2375:	41 83 c5 01          	add    $0x1,%r13d
    2379:	4d 39 f4             	cmp    %r14,%r12
    237c:	74 30                	je     23ae <rio_readlineb+0xaf>
    237e:	8b 53 04             	mov    0x4(%rbx),%edx
    2381:	85 d2                	test   %edx,%edx
    2383:	7e ad                	jle    2332 <rio_readlineb+0x33>
    2385:	eb cb                	jmp    2352 <rio_readlineb+0x53>
    2387:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
    238e:	eb 05                	jmp    2395 <rio_readlineb+0x96>
    2390:	b8 00 00 00 00       	mov    $0x0,%eax
    2395:	85 c0                	test   %eax,%eax
    2397:	75 29                	jne    23c2 <rio_readlineb+0xc3>
    2399:	b8 00 00 00 00       	mov    $0x0,%eax
    239e:	41 83 fd 01          	cmp    $0x1,%r13d
    23a2:	75 0d                	jne    23b1 <rio_readlineb+0xb2>
    23a4:	eb 13                	jmp    23b9 <rio_readlineb+0xba>
    23a6:	41 bd 01 00 00 00    	mov    $0x1,%r13d
    23ac:	eb 03                	jmp    23b1 <rio_readlineb+0xb2>
    23ae:	4d 89 f4             	mov    %r14,%r12
    23b1:	41 c6 04 24 00       	movb   $0x0,(%r12)
    23b6:	49 63 c5             	movslq %r13d,%rax
    23b9:	5b                   	pop    %rbx
    23ba:	5d                   	pop    %rbp
    23bb:	41 5c                	pop    %r12
    23bd:	41 5d                	pop    %r13
    23bf:	41 5e                	pop    %r14
    23c1:	c3                   	ret
    23c2:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
    23c9:	eb ee                	jmp    23b9 <rio_readlineb+0xba>

00000000000023cb <submitr>:
    23cb:	41 57                	push   %r15
    23cd:	41 56                	push   %r14
    23cf:	41 55                	push   %r13
    23d1:	41 54                	push   %r12
    23d3:	55                   	push   %rbp
    23d4:	53                   	push   %rbx
    23d5:	48 81 ec 78 a0 00 00 	sub    $0xa078,%rsp
    23dc:	48 89 7c 24 08       	mov    %rdi,0x8(%rsp)
    23e1:	89 f5                	mov    %esi,%ebp
    23e3:	49 89 d4             	mov    %rdx,%r12
    23e6:	48 89 4c 24 10       	mov    %rcx,0x10(%rsp)
    23eb:	4c 89 44 24 20       	mov    %r8,0x20(%rsp)
    23f0:	4c 89 4c 24 18       	mov    %r9,0x18(%rsp)
    23f5:	48 8b 9c 24 b0 a0 00 	mov    0xa0b0(%rsp),%rbx
    23fc:	00 
    23fd:	4c 8b bc 24 b8 a0 00 	mov    0xa0b8(%rsp),%r15
    2404:	00 
    2405:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    240c:	00 00 
    240e:	48 89 84 24 68 a0 00 	mov    %rax,0xa068(%rsp)
    2415:	00 
    2416:	31 c0                	xor    %eax,%eax
    2418:	c7 44 24 3c 00 00 00 	movl   $0x0,0x3c(%rsp)
    241f:	00 
    2420:	ba 00 00 00 00       	mov    $0x0,%edx
    2425:	be 01 00 00 00       	mov    $0x1,%esi
    242a:	bf 02 00 00 00       	mov    $0x2,%edi
    242f:	e8 9c ed ff ff       	call   11d0 <socket@plt>
    2434:	85 c0                	test   %eax,%eax
    2436:	0f 88 0d 01 00 00    	js     2549 <submitr+0x17e>
    243c:	41 89 c6             	mov    %eax,%r14d
    243f:	48 8b 7c 24 08       	mov    0x8(%rsp),%rdi
    2444:	e8 d7 ec ff ff       	call   1120 <gethostbyname@plt>
    2449:	48 85 c0             	test   %rax,%rax
    244c:	0f 84 47 01 00 00    	je     2599 <submitr+0x1ce>
    2452:	4c 8d 6c 24 40       	lea    0x40(%rsp),%r13
    2457:	48 c7 44 24 40 00 00 	movq   $0x0,0x40(%rsp)
    245e:	00 00 
    2460:	48 c7 44 24 48 00 00 	movq   $0x0,0x48(%rsp)
    2467:	00 00 
    2469:	66 c7 44 24 40 02 00 	movw   $0x2,0x40(%rsp)
    2470:	48 63 50 14          	movslq 0x14(%rax),%rdx
    2474:	48 8b 40 18          	mov    0x18(%rax),%rax
    2478:	48 8d 7c 24 44       	lea    0x44(%rsp),%rdi
    247d:	48 8b 30             	mov    (%rax),%rsi
    2480:	e8 db ec ff ff       	call   1160 <memmove@plt>
    2485:	66 c1 c5 08          	rol    $0x8,%bp
    2489:	66 89 6c 24 42       	mov    %bp,0x42(%rsp)
    248e:	ba 10 00 00 00       	mov    $0x10,%edx
    2493:	4c 89 ee             	mov    %r13,%rsi
    2496:	44 89 f7             	mov    %r14d,%edi
    2499:	e8 02 ed ff ff       	call   11a0 <connect@plt>
    249e:	85 c0                	test   %eax,%eax
    24a0:	0f 88 5e 01 00 00    	js     2604 <submitr+0x239>
    24a6:	48 89 df             	mov    %rbx,%rdi
    24a9:	e8 e2 eb ff ff       	call   1090 <strlen@plt>
    24ae:	48 89 c5             	mov    %rax,%rbp
    24b1:	4c 89 e7             	mov    %r12,%rdi
    24b4:	e8 d7 eb ff ff       	call   1090 <strlen@plt>
    24b9:	49 89 c5             	mov    %rax,%r13
    24bc:	48 8b 7c 24 10       	mov    0x10(%rsp),%rdi
    24c1:	e8 ca eb ff ff       	call   1090 <strlen@plt>
    24c6:	49 89 c4             	mov    %rax,%r12
    24c9:	48 8b 7c 24 18       	mov    0x18(%rsp),%rdi
    24ce:	e8 bd eb ff ff       	call   1090 <strlen@plt>
    24d3:	48 89 c2             	mov    %rax,%rdx
    24d6:	4b 8d 84 25 80 00 00 	lea    0x80(%r13,%r12,1),%rax
    24dd:	00 
    24de:	48 01 d0             	add    %rdx,%rax
    24e1:	48 8d 54 6d 00       	lea    0x0(%rbp,%rbp,2),%rdx
    24e6:	48 01 d0             	add    %rdx,%rax
    24e9:	48 3d 00 20 00 00    	cmp    $0x2000,%rax
    24ef:	0f 87 6c 01 00 00    	ja     2661 <submitr+0x296>
    24f5:	48 8d 94 24 60 40 00 	lea    0x4060(%rsp),%rdx
    24fc:	00 
    24fd:	b9 00 04 00 00       	mov    $0x400,%ecx
    2502:	b8 00 00 00 00       	mov    $0x0,%eax
    2507:	48 89 d7             	mov    %rdx,%rdi
    250a:	f3 48 ab             	rep stos %rax,%es:(%rdi)
    250d:	48 89 df             	mov    %rbx,%rdi
    2510:	e8 7b eb ff ff       	call   1090 <strlen@plt>
    2515:	85 c0                	test   %eax,%eax
    2517:	0f 84 13 05 00 00    	je     2a30 <submitr+0x665>
    251d:	8d 40 ff             	lea    -0x1(%rax),%eax
    2520:	4c 8d 64 03 01       	lea    0x1(%rbx,%rax,1),%r12
    2525:	48 8d ac 24 60 40 00 	lea    0x4060(%rsp),%rbp
    252c:	00 
    252d:	48 8d 84 24 60 80 00 	lea    0x8060(%rsp),%rax
    2534:	00 
    2535:	48 89 44 24 28       	mov    %rax,0x28(%rsp)
    253a:	49 bd d9 ff 00 00 00 	movabs $0x2000000000ffd9,%r13
    2541:	00 20 00 
    2544:	e9 a5 01 00 00       	jmp    26ee <submitr+0x323>
    2549:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    2550:	3a 20 43 
    2553:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    255a:	20 75 6e 
    255d:	49 89 07             	mov    %rax,(%r15)
    2560:	49 89 57 08          	mov    %rdx,0x8(%r15)
    2564:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    256b:	74 6f 20 
    256e:	48 ba 63 72 65 61 74 	movabs $0x7320657461657263,%rdx
    2575:	65 20 73 
    2578:	49 89 47 10          	mov    %rax,0x10(%r15)
    257c:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2580:	41 c7 47 20 6f 63 6b 	movl   $0x656b636f,0x20(%r15)
    2587:	65 
    2588:	66 41 c7 47 24 74 00 	movw   $0x74,0x24(%r15)
    258f:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2594:	e9 6e 03 00 00       	jmp    2907 <submitr+0x53c>
    2599:	48 b8 45 72 72 6f 72 	movabs $0x44203a726f727245,%rax
    25a0:	3a 20 44 
    25a3:	48 ba 4e 53 20 69 73 	movabs $0x6e7520736920534e,%rdx
    25aa:	20 75 6e 
    25ad:	49 89 07             	mov    %rax,(%r15)
    25b0:	49 89 57 08          	mov    %rdx,0x8(%r15)
    25b4:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    25bb:	74 6f 20 
    25be:	48 ba 72 65 73 6f 6c 	movabs $0x2065766c6f736572,%rdx
    25c5:	76 65 20 
    25c8:	49 89 47 10          	mov    %rax,0x10(%r15)
    25cc:	49 89 57 18          	mov    %rdx,0x18(%r15)
    25d0:	48 b8 73 65 72 76 65 	movabs $0x6120726576726573,%rax
    25d7:	72 20 61 
    25da:	49 89 47 20          	mov    %rax,0x20(%r15)
    25de:	41 c7 47 28 64 64 72 	movl   $0x65726464,0x28(%r15)
    25e5:	65 
    25e6:	66 41 c7 47 2c 73 73 	movw   $0x7373,0x2c(%r15)
    25ed:	41 c6 47 2e 00       	movb   $0x0,0x2e(%r15)
    25f2:	44 89 f7             	mov    %r14d,%edi
    25f5:	e8 d6 ea ff ff       	call   10d0 <close@plt>
    25fa:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    25ff:	e9 03 03 00 00       	jmp    2907 <submitr+0x53c>
    2604:	48 b8 45 72 72 6f 72 	movabs $0x55203a726f727245,%rax
    260b:	3a 20 55 
    260e:	48 ba 6e 61 62 6c 65 	movabs $0x6f7420656c62616e,%rdx
    2615:	20 74 6f 
    2618:	49 89 07             	mov    %rax,(%r15)
    261b:	49 89 57 08          	mov    %rdx,0x8(%r15)
    261f:	48 b8 20 63 6f 6e 6e 	movabs $0x7463656e6e6f6320,%rax
    2626:	65 63 74 
    2629:	48 ba 20 74 6f 20 74 	movabs $0x20656874206f7420,%rdx
    2630:	68 65 20 
    2633:	49 89 47 10          	mov    %rax,0x10(%r15)
    2637:	49 89 57 18          	mov    %rdx,0x18(%r15)
    263b:	41 c7 47 20 73 65 72 	movl   $0x76726573,0x20(%r15)
    2642:	76 
    2643:	66 41 c7 47 24 65 72 	movw   $0x7265,0x24(%r15)
    264a:	41 c6 47 26 00       	movb   $0x0,0x26(%r15)
    264f:	44 89 f7             	mov    %r14d,%edi
    2652:	e8 79 ea ff ff       	call   10d0 <close@plt>
    2657:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    265c:	e9 a6 02 00 00       	jmp    2907 <submitr+0x53c>
    2661:	48 b8 45 72 72 6f 72 	movabs $0x52203a726f727245,%rax
    2668:	3a 20 52 
    266b:	48 ba 65 73 75 6c 74 	movabs $0x747320746c757365,%rdx
    2672:	20 73 74 
    2675:	49 89 07             	mov    %rax,(%r15)
    2678:	49 89 57 08          	mov    %rdx,0x8(%r15)
    267c:	48 b8 72 69 6e 67 20 	movabs $0x6f6f7420676e6972,%rax
    2683:	74 6f 6f 
    2686:	48 ba 20 6c 61 72 67 	movabs $0x202e656772616c20,%rdx
    268d:	65 2e 20 
    2690:	49 89 47 10          	mov    %rax,0x10(%r15)
    2694:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2698:	48 b8 49 6e 63 72 65 	movabs $0x6573616572636e49,%rax
    269f:	61 73 65 
    26a2:	48 ba 20 53 55 42 4d 	movabs $0x5254494d42555320,%rdx
    26a9:	49 54 52 
    26ac:	49 89 47 20          	mov    %rax,0x20(%r15)
    26b0:	49 89 57 28          	mov    %rdx,0x28(%r15)
    26b4:	48 b8 5f 4d 41 58 42 	movabs $0x46554258414d5f,%rax
    26bb:	55 46 00 
    26be:	49 89 47 30          	mov    %rax,0x30(%r15)
    26c2:	44 89 f7             	mov    %r14d,%edi
    26c5:	e8 06 ea ff ff       	call   10d0 <close@plt>
    26ca:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    26cf:	e9 33 02 00 00       	jmp    2907 <submitr+0x53c>
    26d4:	49 0f a3 c5          	bt     %rax,%r13
    26d8:	73 1e                	jae    26f8 <submitr+0x32d>
    26da:	88 55 00             	mov    %dl,0x0(%rbp)
    26dd:	48 8d 6d 01          	lea    0x1(%rbp),%rbp
    26e1:	48 83 c3 01          	add    $0x1,%rbx
    26e5:	4c 39 e3             	cmp    %r12,%rbx
    26e8:	0f 84 42 03 00 00    	je     2a30 <submitr+0x665>
    26ee:	0f b6 13             	movzbl (%rbx),%edx
    26f1:	8d 42 d6             	lea    -0x2a(%rdx),%eax
    26f4:	3c 35                	cmp    $0x35,%al
    26f6:	76 dc                	jbe    26d4 <submitr+0x309>
    26f8:	89 d0                	mov    %edx,%eax
    26fa:	83 e0 df             	and    $0xffffffdf,%eax
    26fd:	83 e8 41             	sub    $0x41,%eax
    2700:	3c 19                	cmp    $0x19,%al
    2702:	76 d6                	jbe    26da <submitr+0x30f>
    2704:	80 fa 20             	cmp    $0x20,%dl
    2707:	74 50                	je     2759 <submitr+0x38e>
    2709:	8d 42 e0             	lea    -0x20(%rdx),%eax
    270c:	3c 5f                	cmp    $0x5f,%al
    270e:	76 09                	jbe    2719 <submitr+0x34e>
    2710:	80 fa 09             	cmp    $0x9,%dl
    2713:	0f 85 8a 02 00 00    	jne    29a3 <submitr+0x5d8>
    2719:	0f b6 d2             	movzbl %dl,%edx
    271c:	48 8d 35 2c 10 00 00 	lea    0x102c(%rip),%rsi        # 374f <array.0+0x4cf>
    2723:	48 8b 7c 24 28       	mov    0x28(%rsp),%rdi
    2728:	b8 00 00 00 00       	mov    $0x0,%eax
    272d:	e8 4e ea ff ff       	call   1180 <sprintf@plt>
    2732:	0f b6 84 24 60 80 00 	movzbl 0x8060(%rsp),%eax
    2739:	00 
    273a:	88 45 00             	mov    %al,0x0(%rbp)
    273d:	0f b6 84 24 61 80 00 	movzbl 0x8061(%rsp),%eax
    2744:	00 
    2745:	88 45 01             	mov    %al,0x1(%rbp)
    2748:	0f b6 84 24 62 80 00 	movzbl 0x8062(%rsp),%eax
    274f:	00 
    2750:	88 45 02             	mov    %al,0x2(%rbp)
    2753:	48 8d 6d 03          	lea    0x3(%rbp),%rbp
    2757:	eb 88                	jmp    26e1 <submitr+0x316>
    2759:	c6 45 00 2b          	movb   $0x2b,0x0(%rbp)
    275d:	48 8d 6d 01          	lea    0x1(%rbp),%rbp
    2761:	e9 7b ff ff ff       	jmp    26e1 <submitr+0x316>
    2766:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    276d:	3a 20 43 
    2770:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    2777:	20 75 6e 
    277a:	49 89 07             	mov    %rax,(%r15)
    277d:	49 89 57 08          	mov    %rdx,0x8(%r15)
    2781:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    2788:	74 6f 20 
    278b:	48 ba 77 72 69 74 65 	movabs $0x6f74206574697277,%rdx
    2792:	20 74 6f 
    2795:	49 89 47 10          	mov    %rax,0x10(%r15)
    2799:	49 89 57 18          	mov    %rdx,0x18(%r15)
    279d:	48 b8 20 74 68 65 20 	movabs $0x7265732065687420,%rax
    27a4:	73 65 72 
    27a7:	49 89 47 20          	mov    %rax,0x20(%r15)
    27ab:	41 c7 47 28 76 65 72 	movl   $0x726576,0x28(%r15)
    27b2:	00 
    27b3:	44 89 f7             	mov    %r14d,%edi
    27b6:	e8 15 e9 ff ff       	call   10d0 <close@plt>
    27bb:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    27c0:	e9 42 01 00 00       	jmp    2907 <submitr+0x53c>
    27c5:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    27cc:	3a 20 43 
    27cf:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    27d6:	20 75 6e 
    27d9:	49 89 07             	mov    %rax,(%r15)
    27dc:	49 89 57 08          	mov    %rdx,0x8(%r15)
    27e0:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    27e7:	74 6f 20 
    27ea:	48 ba 77 72 69 74 65 	movabs $0x6f74206574697277,%rdx
    27f1:	20 74 6f 
    27f4:	49 89 47 10          	mov    %rax,0x10(%r15)
    27f8:	49 89 57 18          	mov    %rdx,0x18(%r15)
    27fc:	48 b8 20 74 68 65 20 	movabs $0x7265732065687420,%rax
    2803:	73 65 72 
    2806:	49 89 47 20          	mov    %rax,0x20(%r15)
    280a:	41 c7 47 28 76 65 72 	movl   $0x726576,0x28(%r15)
    2811:	00 
    2812:	44 89 f7             	mov    %r14d,%edi
    2815:	e8 b6 e8 ff ff       	call   10d0 <close@plt>
    281a:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    281f:	e9 e3 00 00 00       	jmp    2907 <submitr+0x53c>
    2824:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    282b:	3a 20 43 
    282e:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    2835:	20 75 6e 
    2838:	49 89 07             	mov    %rax,(%r15)
    283b:	49 89 57 08          	mov    %rdx,0x8(%r15)
    283f:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    2846:	74 6f 20 
    2849:	48 ba 72 65 61 64 20 	movabs $0x7269662064616572,%rdx
    2850:	66 69 72 
    2853:	49 89 47 10          	mov    %rax,0x10(%r15)
    2857:	49 89 57 18          	mov    %rdx,0x18(%r15)
    285b:	48 b8 73 74 20 68 65 	movabs $0x6564616568207473,%rax
    2862:	61 64 65 
    2865:	48 ba 72 20 66 72 6f 	movabs $0x73206d6f72662072,%rdx
    286c:	6d 20 73 
    286f:	49 89 47 20          	mov    %rax,0x20(%r15)
    2873:	49 89 57 28          	mov    %rdx,0x28(%r15)
    2877:	41 c7 47 30 65 72 76 	movl   $0x65767265,0x30(%r15)
    287e:	65 
    287f:	66 41 c7 47 34 72 00 	movw   $0x72,0x34(%r15)
    2886:	44 89 f7             	mov    %r14d,%edi
    2889:	e8 42 e8 ff ff       	call   10d0 <close@plt>
    288e:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2893:	eb 72                	jmp    2907 <submitr+0x53c>
    2895:	48 8d 8c 24 60 80 00 	lea    0x8060(%rsp),%rcx
    289c:	00 
    289d:	48 8d 35 fc 0d 00 00 	lea    0xdfc(%rip),%rsi        # 36a0 <array.0+0x420>
    28a4:	4c 89 ff             	mov    %r15,%rdi
    28a7:	b8 00 00 00 00       	mov    $0x0,%eax
    28ac:	e8 cf e8 ff ff       	call   1180 <sprintf@plt>
    28b1:	44 89 f7             	mov    %r14d,%edi
    28b4:	e8 17 e8 ff ff       	call   10d0 <close@plt>
    28b9:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    28be:	eb 47                	jmp    2907 <submitr+0x53c>
    28c0:	48 8d b4 24 60 20 00 	lea    0x2060(%rsp),%rsi
    28c7:	00 
    28c8:	48 8d 7c 24 50       	lea    0x50(%rsp),%rdi
    28cd:	ba 00 20 00 00       	mov    $0x2000,%edx
    28d2:	e8 28 fa ff ff       	call   22ff <rio_readlineb>
    28d7:	48 85 c0             	test   %rax,%rax
    28da:	7e 54                	jle    2930 <submitr+0x565>
    28dc:	48 8d b4 24 60 20 00 	lea    0x2060(%rsp),%rsi
    28e3:	00 
    28e4:	4c 89 ff             	mov    %r15,%rdi
    28e7:	e8 74 e7 ff ff       	call   1060 <strcpy@plt>
    28ec:	44 89 f7             	mov    %r14d,%edi
    28ef:	e8 dc e7 ff ff       	call   10d0 <close@plt>
    28f4:	48 8d 35 79 0e 00 00 	lea    0xe79(%rip),%rsi        # 3774 <array.0+0x4f4>
    28fb:	4c 89 ff             	mov    %r15,%rdi
    28fe:	e8 fd e7 ff ff       	call   1100 <strcmp@plt>
    2903:	f7 d8                	neg    %eax
    2905:	19 c0                	sbb    %eax,%eax
    2907:	48 8b 94 24 68 a0 00 	mov    0xa068(%rsp),%rdx
    290e:	00 
    290f:	64 48 2b 14 25 28 00 	sub    %fs:0x28,%rdx
    2916:	00 00 
    2918:	0f 85 be 02 00 00    	jne    2bdc <submitr+0x811>
    291e:	48 81 c4 78 a0 00 00 	add    $0xa078,%rsp
    2925:	5b                   	pop    %rbx
    2926:	5d                   	pop    %rbp
    2927:	41 5c                	pop    %r12
    2929:	41 5d                	pop    %r13
    292b:	41 5e                	pop    %r14
    292d:	41 5f                	pop    %r15
    292f:	c3                   	ret
    2930:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    2937:	3a 20 43 
    293a:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    2941:	20 75 6e 
    2944:	49 89 07             	mov    %rax,(%r15)
    2947:	49 89 57 08          	mov    %rdx,0x8(%r15)
    294b:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    2952:	74 6f 20 
    2955:	48 ba 72 65 61 64 20 	movabs $0x6174732064616572,%rdx
    295c:	73 74 61 
    295f:	49 89 47 10          	mov    %rax,0x10(%r15)
    2963:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2967:	48 b8 74 75 73 20 6d 	movabs $0x7373656d20737574,%rax
    296e:	65 73 73 
    2971:	48 ba 61 67 65 20 66 	movabs $0x6d6f726620656761,%rdx
    2978:	72 6f 6d 
    297b:	49 89 47 20          	mov    %rax,0x20(%r15)
    297f:	49 89 57 28          	mov    %rdx,0x28(%r15)
    2983:	48 b8 20 73 65 72 76 	movabs $0x72657672657320,%rax
    298a:	65 72 00 
    298d:	49 89 47 30          	mov    %rax,0x30(%r15)
    2991:	44 89 f7             	mov    %r14d,%edi
    2994:	e8 37 e7 ff ff       	call   10d0 <close@plt>
    2999:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    299e:	e9 64 ff ff ff       	jmp    2907 <submitr+0x53c>
    29a3:	48 b8 45 72 72 6f 72 	movabs $0x52203a726f727245,%rax
    29aa:	3a 20 52 
    29ad:	48 ba 65 73 75 6c 74 	movabs $0x747320746c757365,%rdx
    29b4:	20 73 74 
    29b7:	49 89 07             	mov    %rax,(%r15)
    29ba:	49 89 57 08          	mov    %rdx,0x8(%r15)
    29be:	48 b8 72 69 6e 67 20 	movabs $0x6e6f6320676e6972,%rax
    29c5:	63 6f 6e 
    29c8:	48 ba 74 61 69 6e 73 	movabs $0x6e6120736e696174,%rdx
    29cf:	20 61 6e 
    29d2:	49 89 47 10          	mov    %rax,0x10(%r15)
    29d6:	49 89 57 18          	mov    %rdx,0x18(%r15)
    29da:	48 b8 20 69 6c 6c 65 	movabs $0x6c6167656c6c6920,%rax
    29e1:	67 61 6c 
    29e4:	48 ba 20 6f 72 20 75 	movabs $0x72706e7520726f20,%rdx
    29eb:	6e 70 72 
    29ee:	49 89 47 20          	mov    %rax,0x20(%r15)
    29f2:	49 89 57 28          	mov    %rdx,0x28(%r15)
    29f6:	48 b8 69 6e 74 61 62 	movabs $0x20656c6261746e69,%rax
    29fd:	6c 65 20 
    2a00:	48 ba 63 68 61 72 61 	movabs $0x6574636172616863,%rdx
    2a07:	63 74 65 
    2a0a:	49 89 47 30          	mov    %rax,0x30(%r15)
    2a0e:	49 89 57 38          	mov    %rdx,0x38(%r15)
    2a12:	66 41 c7 47 40 72 2e 	movw   $0x2e72,0x40(%r15)
    2a19:	41 c6 47 42 00       	movb   $0x0,0x42(%r15)
    2a1e:	44 89 f7             	mov    %r14d,%edi
    2a21:	e8 aa e6 ff ff       	call   10d0 <close@plt>
    2a26:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2a2b:	e9 d7 fe ff ff       	jmp    2907 <submitr+0x53c>
    2a30:	48 8d 9c 24 60 20 00 	lea    0x2060(%rsp),%rbx
    2a37:	00 
    2a38:	4c 8d 8c 24 60 40 00 	lea    0x4060(%rsp),%r9
    2a3f:	00 
    2a40:	4c 8b 44 24 18       	mov    0x18(%rsp),%r8
    2a45:	48 8b 4c 24 20       	mov    0x20(%rsp),%rcx
    2a4a:	48 8b 54 24 10       	mov    0x10(%rsp),%rdx
    2a4f:	48 8d 35 7a 0c 00 00 	lea    0xc7a(%rip),%rsi        # 36d0 <array.0+0x450>
    2a56:	48 89 df             	mov    %rbx,%rdi
    2a59:	b8 00 00 00 00       	mov    $0x0,%eax
    2a5e:	e8 1d e7 ff ff       	call   1180 <sprintf@plt>
    2a63:	48 89 df             	mov    %rbx,%rdi
    2a66:	e8 25 e6 ff ff       	call   1090 <strlen@plt>
    2a6b:	48 89 c2             	mov    %rax,%rdx
    2a6e:	48 89 de             	mov    %rbx,%rsi
    2a71:	44 89 f7             	mov    %r14d,%edi
    2a74:	e8 26 f8 ff ff       	call   229f <rio_writen>
    2a79:	48 85 c0             	test   %rax,%rax
    2a7c:	0f 88 e4 fc ff ff    	js     2766 <submitr+0x39b>
    2a82:	48 8d 9c 24 60 20 00 	lea    0x2060(%rsp),%rbx
    2a89:	00 
    2a8a:	48 8b 54 24 08       	mov    0x8(%rsp),%rdx
    2a8f:	48 8d 35 c0 0c 00 00 	lea    0xcc0(%rip),%rsi        # 3756 <array.0+0x4d6>
    2a96:	48 89 df             	mov    %rbx,%rdi
    2a99:	b8 00 00 00 00       	mov    $0x0,%eax
    2a9e:	e8 dd e6 ff ff       	call   1180 <sprintf@plt>
    2aa3:	48 89 df             	mov    %rbx,%rdi
    2aa6:	e8 e5 e5 ff ff       	call   1090 <strlen@plt>
    2aab:	48 89 c2             	mov    %rax,%rdx
    2aae:	48 89 de             	mov    %rbx,%rsi
    2ab1:	44 89 f7             	mov    %r14d,%edi
    2ab4:	e8 e6 f7 ff ff       	call   229f <rio_writen>
    2ab9:	48 85 c0             	test   %rax,%rax
    2abc:	0f 88 03 fd ff ff    	js     27c5 <submitr+0x3fa>
    2ac2:	44 89 74 24 50       	mov    %r14d,0x50(%rsp)
    2ac7:	c7 44 24 54 00 00 00 	movl   $0x0,0x54(%rsp)
    2ace:	00 
    2acf:	48 8d 7c 24 50       	lea    0x50(%rsp),%rdi
    2ad4:	48 8d 44 24 60       	lea    0x60(%rsp),%rax
    2ad9:	48 89 44 24 58       	mov    %rax,0x58(%rsp)
    2ade:	48 8d b4 24 60 20 00 	lea    0x2060(%rsp),%rsi
    2ae5:	00 
    2ae6:	ba 00 20 00 00       	mov    $0x2000,%edx
    2aeb:	e8 0f f8 ff ff       	call   22ff <rio_readlineb>
    2af0:	48 85 c0             	test   %rax,%rax
    2af3:	0f 8e 2b fd ff ff    	jle    2824 <submitr+0x459>
    2af9:	48 8d 4c 24 3c       	lea    0x3c(%rsp),%rcx
    2afe:	48 8d 94 24 60 60 00 	lea    0x6060(%rsp),%rdx
    2b05:	00 
    2b06:	48 8d bc 24 60 20 00 	lea    0x2060(%rsp),%rdi
    2b0d:	00 
    2b0e:	4c 8d 84 24 60 80 00 	lea    0x8060(%rsp),%r8
    2b15:	00 
    2b16:	48 8d 35 46 0c 00 00 	lea    0xc46(%rip),%rsi        # 3763 <array.0+0x4e3>
    2b1d:	b8 00 00 00 00       	mov    $0x0,%eax
    2b22:	e8 29 e6 ff ff       	call   1150 <__isoc99_sscanf@plt>
    2b27:	8b 54 24 3c          	mov    0x3c(%rsp),%edx
    2b2b:	81 fa c8 00 00 00    	cmp    $0xc8,%edx
    2b31:	0f 85 5e fd ff ff    	jne    2895 <submitr+0x4ca>
    2b37:	48 8d 1d 22 0c 00 00 	lea    0xc22(%rip),%rbx        # 3760 <array.0+0x4e0>
    2b3e:	48 8d bc 24 60 20 00 	lea    0x2060(%rsp),%rdi
    2b45:	00 
    2b46:	48 89 de             	mov    %rbx,%rsi
    2b49:	e8 b2 e5 ff ff       	call   1100 <strcmp@plt>
    2b4e:	85 c0                	test   %eax,%eax
    2b50:	0f 84 6a fd ff ff    	je     28c0 <submitr+0x4f5>
    2b56:	48 8d b4 24 60 20 00 	lea    0x2060(%rsp),%rsi
    2b5d:	00 
    2b5e:	48 8d 7c 24 50       	lea    0x50(%rsp),%rdi
    2b63:	ba 00 20 00 00       	mov    $0x2000,%edx
    2b68:	e8 92 f7 ff ff       	call   22ff <rio_readlineb>
    2b6d:	48 85 c0             	test   %rax,%rax
    2b70:	7f cc                	jg     2b3e <submitr+0x773>
    2b72:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    2b79:	3a 20 43 
    2b7c:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    2b83:	20 75 6e 
    2b86:	49 89 07             	mov    %rax,(%r15)
    2b89:	49 89 57 08          	mov    %rdx,0x8(%r15)
    2b8d:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    2b94:	74 6f 20 
    2b97:	48 ba 72 65 61 64 20 	movabs $0x6165682064616572,%rdx
    2b9e:	68 65 61 
    2ba1:	49 89 47 10          	mov    %rax,0x10(%r15)
    2ba5:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2ba9:	48 b8 64 65 72 73 20 	movabs $0x6f72662073726564,%rax
    2bb0:	66 72 6f 
    2bb3:	48 ba 6d 20 73 65 72 	movabs $0x726576726573206d,%rdx
    2bba:	76 65 72 
    2bbd:	49 89 47 20          	mov    %rax,0x20(%r15)
    2bc1:	49 89 57 28          	mov    %rdx,0x28(%r15)
    2bc5:	41 c6 47 30 00       	movb   $0x0,0x30(%r15)
    2bca:	44 89 f7             	mov    %r14d,%edi
    2bcd:	e8 fe e4 ff ff       	call   10d0 <close@plt>
    2bd2:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2bd7:	e9 2b fd ff ff       	jmp    2907 <submitr+0x53c>
    2bdc:	e8 bf e4 ff ff       	call   10a0 <__stack_chk_fail@plt>

0000000000002be1 <init_timeout>:
    2be1:	85 ff                	test   %edi,%edi
    2be3:	75 01                	jne    2be6 <init_timeout+0x5>
    2be5:	c3                   	ret
    2be6:	53                   	push   %rbx
    2be7:	89 fb                	mov    %edi,%ebx
    2be9:	48 8d 35 84 f6 ff ff 	lea    -0x97c(%rip),%rsi        # 2274 <sigalrm_handler>
    2bf0:	bf 0e 00 00 00       	mov    $0xe,%edi
    2bf5:	e8 16 e5 ff ff       	call   1110 <signal@plt>
    2bfa:	85 db                	test   %ebx,%ebx
    2bfc:	b8 00 00 00 00       	mov    $0x0,%eax
    2c01:	0f 49 c3             	cmovns %ebx,%eax
    2c04:	89 c7                	mov    %eax,%edi
    2c06:	e8 b5 e4 ff ff       	call   10c0 <alarm@plt>
    2c0b:	5b                   	pop    %rbx
    2c0c:	c3                   	ret

0000000000002c0d <init_driver>:
    2c0d:	41 54                	push   %r12
    2c0f:	55                   	push   %rbp
    2c10:	53                   	push   %rbx
    2c11:	48 83 ec 20          	sub    $0x20,%rsp
    2c15:	48 89 fd             	mov    %rdi,%rbp
    2c18:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    2c1f:	00 00 
    2c21:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
    2c26:	31 c0                	xor    %eax,%eax
    2c28:	be 01 00 00 00       	mov    $0x1,%esi
    2c2d:	bf 0d 00 00 00       	mov    $0xd,%edi
    2c32:	e8 d9 e4 ff ff       	call   1110 <signal@plt>
    2c37:	be 01 00 00 00       	mov    $0x1,%esi
    2c3c:	bf 1d 00 00 00       	mov    $0x1d,%edi
    2c41:	e8 ca e4 ff ff       	call   1110 <signal@plt>
    2c46:	be 01 00 00 00       	mov    $0x1,%esi
    2c4b:	bf 1d 00 00 00       	mov    $0x1d,%edi
    2c50:	e8 bb e4 ff ff       	call   1110 <signal@plt>
    2c55:	ba 00 00 00 00       	mov    $0x0,%edx
    2c5a:	be 01 00 00 00       	mov    $0x1,%esi
    2c5f:	bf 02 00 00 00       	mov    $0x2,%edi
    2c64:	e8 67 e5 ff ff       	call   11d0 <socket@plt>
    2c69:	85 c0                	test   %eax,%eax
    2c6b:	0f 88 97 00 00 00    	js     2d08 <init_driver+0xfb>
    2c71:	89 c3                	mov    %eax,%ebx
    2c73:	48 8d 3d fd 0a 00 00 	lea    0xafd(%rip),%rdi        # 3777 <array.0+0x4f7>
    2c7a:	e8 a1 e4 ff ff       	call   1120 <gethostbyname@plt>
    2c7f:	48 85 c0             	test   %rax,%rax
    2c82:	0f 84 cc 00 00 00    	je     2d54 <init_driver+0x147>
    2c88:	49 89 e4             	mov    %rsp,%r12
    2c8b:	48 c7 04 24 00 00 00 	movq   $0x0,(%rsp)
    2c92:	00 
    2c93:	48 c7 44 24 08 00 00 	movq   $0x0,0x8(%rsp)
    2c9a:	00 00 
    2c9c:	66 c7 04 24 02 00    	movw   $0x2,(%rsp)
    2ca2:	48 63 50 14          	movslq 0x14(%rax),%rdx
    2ca6:	48 8b 40 18          	mov    0x18(%rax),%rax
    2caa:	48 8d 7c 24 04       	lea    0x4(%rsp),%rdi
    2caf:	48 8b 30             	mov    (%rax),%rsi
    2cb2:	e8 a9 e4 ff ff       	call   1160 <memmove@plt>
    2cb7:	66 c7 44 24 02 00 50 	movw   $0x5000,0x2(%rsp)
    2cbe:	ba 10 00 00 00       	mov    $0x10,%edx
    2cc3:	4c 89 e6             	mov    %r12,%rsi
    2cc6:	89 df                	mov    %ebx,%edi
    2cc8:	e8 d3 e4 ff ff       	call   11a0 <connect@plt>
    2ccd:	85 c0                	test   %eax,%eax
    2ccf:	0f 88 e7 00 00 00    	js     2dbc <init_driver+0x1af>
    2cd5:	89 df                	mov    %ebx,%edi
    2cd7:	e8 f4 e3 ff ff       	call   10d0 <close@plt>
    2cdc:	66 c7 45 00 4f 4b    	movw   $0x4b4f,0x0(%rbp)
    2ce2:	c6 45 02 00          	movb   $0x0,0x2(%rbp)
    2ce6:	b8 00 00 00 00       	mov    $0x0,%eax
    2ceb:	48 8b 54 24 18       	mov    0x18(%rsp),%rdx
    2cf0:	64 48 2b 14 25 28 00 	sub    %fs:0x28,%rdx
    2cf7:	00 00 
    2cf9:	0f 85 ee 00 00 00    	jne    2ded <init_driver+0x1e0>
    2cff:	48 83 c4 20          	add    $0x20,%rsp
    2d03:	5b                   	pop    %rbx
    2d04:	5d                   	pop    %rbp
    2d05:	41 5c                	pop    %r12
    2d07:	c3                   	ret
    2d08:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    2d0f:	3a 20 43 
    2d12:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    2d19:	20 75 6e 
    2d1c:	48 89 45 00          	mov    %rax,0x0(%rbp)
    2d20:	48 89 55 08          	mov    %rdx,0x8(%rbp)
    2d24:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    2d2b:	74 6f 20 
    2d2e:	48 ba 63 72 65 61 74 	movabs $0x7320657461657263,%rdx
    2d35:	65 20 73 
    2d38:	48 89 45 10          	mov    %rax,0x10(%rbp)
    2d3c:	48 89 55 18          	mov    %rdx,0x18(%rbp)
    2d40:	c7 45 20 6f 63 6b 65 	movl   $0x656b636f,0x20(%rbp)
    2d47:	66 c7 45 24 74 00    	movw   $0x74,0x24(%rbp)
    2d4d:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2d52:	eb 97                	jmp    2ceb <init_driver+0xde>
    2d54:	48 b8 45 72 72 6f 72 	movabs $0x44203a726f727245,%rax
    2d5b:	3a 20 44 
    2d5e:	48 ba 4e 53 20 69 73 	movabs $0x6e7520736920534e,%rdx
    2d65:	20 75 6e 
    2d68:	48 89 45 00          	mov    %rax,0x0(%rbp)
    2d6c:	48 89 55 08          	mov    %rdx,0x8(%rbp)
    2d70:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    2d77:	74 6f 20 
    2d7a:	48 ba 72 65 73 6f 6c 	movabs $0x2065766c6f736572,%rdx
    2d81:	76 65 20 
    2d84:	48 89 45 10          	mov    %rax,0x10(%rbp)
    2d88:	48 89 55 18          	mov    %rdx,0x18(%rbp)
    2d8c:	48 b8 73 65 72 76 65 	movabs $0x6120726576726573,%rax
    2d93:	72 20 61 
    2d96:	48 89 45 20          	mov    %rax,0x20(%rbp)
    2d9a:	c7 45 28 64 64 72 65 	movl   $0x65726464,0x28(%rbp)
    2da1:	66 c7 45 2c 73 73    	movw   $0x7373,0x2c(%rbp)
    2da7:	c6 45 2e 00          	movb   $0x0,0x2e(%rbp)
    2dab:	89 df                	mov    %ebx,%edi
    2dad:	e8 1e e3 ff ff       	call   10d0 <close@plt>
    2db2:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2db7:	e9 2f ff ff ff       	jmp    2ceb <init_driver+0xde>
    2dbc:	b9 50 00 00 00       	mov    $0x50,%ecx
    2dc1:	48 8d 15 af 09 00 00 	lea    0x9af(%rip),%rdx        # 3777 <array.0+0x4f7>
    2dc8:	48 8d 35 51 09 00 00 	lea    0x951(%rip),%rsi        # 3720 <array.0+0x4a0>
    2dcf:	48 89 ef             	mov    %rbp,%rdi
    2dd2:	b8 00 00 00 00       	mov    $0x0,%eax
    2dd7:	e8 a4 e3 ff ff       	call   1180 <sprintf@plt>
    2ddc:	89 df                	mov    %ebx,%edi
    2dde:	e8 ed e2 ff ff       	call   10d0 <close@plt>
    2de3:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2de8:	e9 fe fe ff ff       	jmp    2ceb <init_driver+0xde>
    2ded:	e8 ae e2 ff ff       	call   10a0 <__stack_chk_fail@plt>

0000000000002df2 <driver_post>:
    2df2:	53                   	push   %rbx
    2df3:	4c 89 c3             	mov    %r8,%rbx
    2df6:	85 c9                	test   %ecx,%ecx
    2df8:	75 17                	jne    2e11 <driver_post+0x1f>
    2dfa:	48 85 ff             	test   %rdi,%rdi
    2dfd:	74 05                	je     2e04 <driver_post+0x12>
    2dff:	80 3f 00             	cmpb   $0x0,(%rdi)
    2e02:	75 31                	jne    2e35 <driver_post+0x43>
    2e04:	66 c7 03 4f 4b       	movw   $0x4b4f,(%rbx)
    2e09:	c6 43 02 00          	movb   $0x0,0x2(%rbx)
    2e0d:	89 c8                	mov    %ecx,%eax
    2e0f:	5b                   	pop    %rbx
    2e10:	c3                   	ret
    2e11:	48 89 d6             	mov    %rdx,%rsi
    2e14:	48 8d 3d 67 09 00 00 	lea    0x967(%rip),%rdi        # 3782 <array.0+0x502>
    2e1b:	b8 00 00 00 00       	mov    $0x0,%eax
    2e20:	e8 8b e2 ff ff       	call   10b0 <printf@plt>
    2e25:	66 c7 03 4f 4b       	movw   $0x4b4f,(%rbx)
    2e2a:	c6 43 02 00          	movb   $0x0,0x2(%rbx)
    2e2e:	b8 00 00 00 00       	mov    $0x0,%eax
    2e33:	eb da                	jmp    2e0f <driver_post+0x1d>
    2e35:	41 50                	push   %r8
    2e37:	52                   	push   %rdx
    2e38:	4c 8d 0d 5a 09 00 00 	lea    0x95a(%rip),%r9        # 3799 <array.0+0x519>
    2e3f:	49 89 f0             	mov    %rsi,%r8
    2e42:	48 89 f9             	mov    %rdi,%rcx
    2e45:	48 8d 15 55 09 00 00 	lea    0x955(%rip),%rdx        # 37a1 <array.0+0x521>
    2e4c:	be 50 00 00 00       	mov    $0x50,%esi
    2e51:	48 8d 3d 1f 09 00 00 	lea    0x91f(%rip),%rdi        # 3777 <array.0+0x4f7>
    2e58:	e8 6e f5 ff ff       	call   23cb <submitr>
    2e5d:	48 83 c4 10          	add    $0x10,%rsp
    2e61:	eb ac                	jmp    2e0f <driver_post+0x1d>
    2e63:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    2e6a:	00 00 00 
    2e6d:	0f 1f 00             	nopl   (%rax)

0000000000002e70 <__libc_csu_init>:
    2e70:	f3 0f 1e fa          	endbr64
    2e74:	41 57                	push   %r15
    2e76:	4c 8d 3d 6b 2f 00 00 	lea    0x2f6b(%rip),%r15        # 5de8 <__frame_dummy_init_array_entry>
    2e7d:	41 56                	push   %r14
    2e7f:	49 89 d6             	mov    %rdx,%r14
    2e82:	41 55                	push   %r13
    2e84:	49 89 f5             	mov    %rsi,%r13
    2e87:	41 54                	push   %r12
    2e89:	41 89 fc             	mov    %edi,%r12d
    2e8c:	55                   	push   %rbp
    2e8d:	48 8d 2d 5c 2f 00 00 	lea    0x2f5c(%rip),%rbp        # 5df0 <__do_global_dtors_aux_fini_array_entry>
    2e94:	53                   	push   %rbx
    2e95:	4c 29 fd             	sub    %r15,%rbp
    2e98:	48 83 ec 08          	sub    $0x8,%rsp
    2e9c:	e8 5f e1 ff ff       	call   1000 <_init>
    2ea1:	48 c1 fd 03          	sar    $0x3,%rbp
    2ea5:	74 1f                	je     2ec6 <__libc_csu_init+0x56>
    2ea7:	31 db                	xor    %ebx,%ebx
    2ea9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    2eb0:	4c 89 f2             	mov    %r14,%rdx
    2eb3:	4c 89 ee             	mov    %r13,%rsi
    2eb6:	44 89 e7             	mov    %r12d,%edi
    2eb9:	41 ff 14 df          	call   *(%r15,%rbx,8)
    2ebd:	48 83 c3 01          	add    $0x1,%rbx
    2ec1:	48 39 dd             	cmp    %rbx,%rbp
    2ec4:	75 ea                	jne    2eb0 <__libc_csu_init+0x40>
    2ec6:	48 83 c4 08          	add    $0x8,%rsp
    2eca:	5b                   	pop    %rbx
    2ecb:	5d                   	pop    %rbp
    2ecc:	41 5c                	pop    %r12
    2ece:	41 5d                	pop    %r13
    2ed0:	41 5e                	pop    %r14
    2ed2:	41 5f                	pop    %r15
    2ed4:	c3                   	ret
    2ed5:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
    2edc:	00 00 00 00 

0000000000002ee0 <__libc_csu_fini>:
    2ee0:	f3 0f 1e fa          	endbr64
    2ee4:	c3                   	ret

Disassembly of section .fini:

0000000000002ee8 <_fini>:
    2ee8:	f3 0f 1e fa          	endbr64
    2eec:	48 83 ec 08          	sub    $0x8,%rsp
    2ef0:	48 83 c4 08          	add    $0x8,%rsp
    2ef4:	c3                   	ret
