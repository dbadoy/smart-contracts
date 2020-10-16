<h1 class="contract">upsert</h1>
---
spec-version: 0.0.1 <br>
title: Upsert <br>
summary:알리바이에 경력사항을 입력합니다. <br><br>

[upsert]<br>
파라미터 ( user, manager_name, store_name, start_day, end_day, retire_day, working_days )<br>

1:: {{ user }} 와 {{ manager_name}}은 {{ user }}가 {{ store_name }} 에서 {{ start_day }} 부터 {{ end_day}} 까지  근무하는것에 동의한다.<br>

2:: {{ user }}가 퇴직한 날  {{ retire_day }} 를 입력하고 {{ working_day }} 는 {{ start_day }} - {{ retire_day }}로 자동으로 입력된다.<br>

3:: {{ end_day }}와 {{ retire_day }} 가 일치하지 않을 시, 계약의 불이행으로 간주하여 해당 계약서 claim에 근무기간 불이행 내용을 추가한다.<br><br>



<h1 class="contract">issuance</h1>
---
spec-version: 0.0.1 <br>
title: Issuance <br>
summary:허가된 권한의 searcher가  user의 경력을 검색할 수 있게 합니다. <br><br>

[issuance]<br>
파라미터 ( searcher, user, storename)<br>

1:: {{ searcher }}는 {{ user }} 가 {{ store_name }} 에서 근무한 증명서를 불러온다.  증명서에는 {{ user }}, {{ store_name }}, {{ start_day }}, {{ end_day }}, {{ retire_day}}, {{working_day}}, {{ claim }}가 포함 된다.<br>

2:: {{ searcher }}는 {{ user }} 가 발급 허가를 한 사용자이어야 하며, 허가받지 않은 사용자일 경우, 발급이 취소된다.<br><br>




<h1 class="contract">claim</h1>
---
spec-version: 0.0.1 <br>
title: Claim <br> 
summary:근무 과정에 문제 발생시 claim을 걸수있습니다. <br><br>

[claim]<br>
파라미터 ( claimer, target, , store_name, content )<br>


1:: {{ claimer }}는 {{ target }}이 {{ store_name}}에서 작성한 계약서에 불이행되었거나 불합리한 처우를 받았던 것, 기타사항을 기입할 수 있다. <br>

2:: {{ claimer }}는 {{ target }}이 {{ store_name}}에서 작성한 계약서와 관련된 사람이어야 한다. 계약서 상 {{ user }}나 {{ manager_name }} 이 아닌, 제 3자가 claim을 기입 하려고 하는 경우 입력이 취소된다.<br>

3:: {{ claimer }}는 {{ content }}의 내용에 사실만을 적으며, 만약 이에 거짓이 있고 이로인해 상대방이 피해를 입었을 경우 책임은 {{ claimer }}에게 있음을 인지하고 있다.<br><br>




<h1 class="contract">editacl</h1>
---
spec-version: 0.0.1 <br>
title: Editacl <br> 
summary:경력증명서를 볼 수 있는 권한을 부여합니다. <br><br>

[acl]<br>
파라미터 ( user, searcher, flag )<br>


1:: {{ user }}는 {{ flag }}에 값을 이용하여 발급 허가 목록에 {{ searcher }}를 추가 및 삭제 할 수 있다.<br>

2:: {{ user }}는 {{ searcher }}가 자신의 발급 허가 목록에서 삭제되기 전까지 본인의 경력증명서를 조회하는 것을 동의한다.<br>

3:: {{ user }}는 언제든지 {{ searcher }}를 발급 허가 목록에서 삭제할 수 있다.<br><br>
