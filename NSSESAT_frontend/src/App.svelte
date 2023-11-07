<div class="banner">
    NSSESAT
</div>

{#if isSearching}
	<img src="pen.gif" alt="Loading animation" class="image-style">
{/if}

<script>
	import { Scatter } from 'svelte-chartjs';
	import { Chart, LinearScale } from 'chart.js';
	import { PointElement } from 'chart.js';
	import { onMount } from 'svelte';

	Chart.register(PointElement);
	Chart.register(LinearScale);

	let dataFromAPI = {plotdata: []};
	let chartData = {
		datasets: [{
			label: 'データセット1',
			data: [],
			backgroundColor: 'rgba(32, 178, 170, 1)',
			borderColor: 'rgba(32, 178, 170, 1)',
			borderWidth: 1
		}]
	};

	let imageBase64 = "";
	let genValue = ''; // テキストボックスの値を保存する変数
	let errorRate = 0;
	let nodeCount = 0;
	let isSearching = false;

	async function submitForm(event) {
		event.preventDefault();
		const data = new FormData(event.target);
		const response = await fetch('http://127.0.0.1:8000/uploadfile/', {
			method: "POST",
			body: data,
		});
		const result = await response.json();
		console.log(result);

		// 送信が成功したら、"送信完了"と表示する
		if(response.ok) {
			document.getElementById("message").innerText = "保存完了";
		} else {
			document.getElementById("message").innerText = "保存失敗";
		}
	}
	
	let showAnimation = false;

	async function compileAndRunCpp() {
        isSearching = true;  // 探索を開始
        try {
            const response = await fetch("http://127.0.0.1:8000/compile_and_run_cpp", {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({gen: genValue}) // gen値をサーバーに送信
            });
            if (response.ok) {
                const data = await response.json();
                console.log(data.message, data.output);
                if (data.fetchRequired) {
                    fetchData();
                }
            } else {
                console.error(`API Error: ${response.statusText}`);
            }
        } catch (error) {
            console.error(`Fetch Error: ${error}`);
        } finally {
            isSearching = false;  // 探索を終了
        }
    }
	
	async function sendPointData(point) {
		try {
			const response = await fetch("http://127.0.0.1:8000/set_point", {
				method: 'POST',
				headers: {
					'Content-Type': 'application/json'
				},
				body: JSON.stringify({ clicked_point: point })
			});
			if (response.ok) {
				const data = await response.json();
				console.log(data.message);
				imageBase64 = data.image;  // 画像データを取得
				errorRate = data["誤り率"];  // 誤り率を取得
				nodeCount = data["ノード数"];  // ノード数を取得
			} else {
				console.error(`API Error: ${response.statusText}`);
			}
		} catch (error) {
			console.error(`Fetch Error: ${error}`);
		}
	}

	let options = {
		onClick: function(event, elements) {
			if (elements.length > 0) {
				const chartElement = elements[0];
				const dataIndex = chartElement.index;
				const datasetIndex = chartElement.datasetIndex;
				const clickedData = chartData.datasets[datasetIndex].data[dataIndex];
				const pointString = `${clickedData.x}-${clickedData.y}`;
				console.log("Clicked Point:", pointString);
				sendPointData(pointString);  // ここでデータを送信
			}
		}
	};

	async function fetchData() {
		try {
			const response = await fetch("http://127.0.0.1:8000/get_data");
			if (response.ok) {
				dataFromAPI = await response.json();
				chartData.datasets[0].data = dataFromAPI.plotdata;
				console.log("Fetched Data: ", dataFromAPI);
			} else {
				console.error(`API Error: ${response.statusText}`);
			}
		} catch (error) {
			console.error(`Fetch Error: ${error}`);
		}
	}

	onMount(fetchData);
</script>

<style>
	:global(body) {
		padding: 0px;
		background-color: rgba(32, 178, 170, 0.1);
	}

	.searching-text {
		font-size: 50px;
		font-weight: bold; 
		text-align: center; 
		position: fixed; 

		top: 50%;
		left: 50%;
		transform: translate(-50%, -50%);
	}

	.table1 {
		font-size: 24px; /* テキストのサイズ */
	}

	.input-container {
		display: flex;
		justify-content: center; /* 水平方向の中央揃え */
		align-items: center; /* 垂直方向の中央揃え */
		margin-top: 20px; /* 上との隙間を20pxに設定 */
	}

	.input-container input, .input-container button {
		font-size: 1.5em;          /* フォントサイズを大きくする */
		padding: 10px 20px;       /* パディングを追加して要素のサイズを大きくする */
		margin: 10px;              /* 要素間の隙間 */
	}

	.input-container input {
		width: 250px;             /* テキストボックスの幅を設定 */
		border: 1px solid #ccc;  /* テキストボックスの枠線 */
		border-radius: 5px;      /* 角を少し丸くする */
	}

	.input-container button {
		background-color: rgba(32, 178, 170, 0.7);/* ボタンの背景色 */
		color: white;             /* ボタンのテキストカラー */
		border: none;             /* ボタンの枠線を削除 */
		border-radius: 5px;      /* 角を少し丸くする */
		cursor: pointer;         /* ホバー時のカーソルを指アイコンにする */
	}

	.input-container button:hover {
		background-color: #0056b3;/* ホバー時のボタンの背景色 */
	}

	.banner {
		background-color: rgba(32, 178, 170, 0.7); /* 透明度を0.5に設定 */
		height: 60px; /* バナーの高さ */
		width: 100%; /* バナーの幅 */
		display: flex;
		justify-content: center; /* コンテンツを中央に配置 */
		align-items: center; /* コンテンツを垂直に中央に配置 */
		font-size: 36px; /* テキストのサイズ */
		color: white; /* テキストの色 */
	}


	.img_container {
		width: 50%;
		height: 100%;
		display: flex;
		justify-content: center;
		align-items: center;
	}

	img {
		display: block;
		max-width: 100%;
		width: auto;
		max-height: 100%;
		margin: 0;  /* 画像のマージンを0に設定 */
	}

	.flex-container {
		display: flex;
		justify-content: flex-start;
		align-items: center;
		height: calc(100vh - 70px);
	}
	th, td {
		text-align: center;
	}
	table {
    border-collapse: collapse;  /* これにより、隣接するセルの枠線が重なる */
	}

	th, td {
		border: 1px solid black;  /* 枠線を追加 */
		text-align: center;       /* テキストを中央揃えにする */
		padding: 8px;             /* セル内のテキストと枠線の間にスペースを追加 */
	}
	.container {
		display: flex;
		justify-content: center;
		align-items: center;
	}

	.center {
		display: flex;
		justify-content: center;
		align-items: center;
		flex-direction: column;
		border: 1px solid #ccc; /* 枠で囲む */
	}

	.form-container {
		text-align: center; /* フォーム内の要素を中央寄せ */
	}

	button {
		background-color: rgba(32, 178, 170, 0.7); /* 背景色 */
		color: white; /* テキスト色 */
		border: none; /* ボーダーを削除 */
		padding: 10px 20px; /* パディング */
		cursor: pointer; /* カーソル */
	}

	button:hover {
		background-color: rgba(32, 178, 170, 0.9); /* ホバー時の背景色 */
	}

	.main-container {
		display: flex;
		justify-content: center;
		align-items: center;
		flex-direction: row; /* 横並びにする */
	}

	/* 既存のCSSに変更なし */
	.container {
		display: flex;
		justify-content: center;
		align-items: center;
	}
	.image-style {
		display: block; /* 画像をブロック要素として扱う */
		margin: 0 auto; /* 左右のマージンを自動で均等にして中央揃え */
	}
</style>


<div class="flex-container">
  
<div class="chart-container" style="padding: 15px; display: block; width: 50% !important; margin: 0;">
	<div class="inner-container" style="border: 2px solid rgba(32, 178, 170, 0.6); background-color: rgba(255, 255, 255, 0.5); width: 100%; height: 100%;">
		<div class="main-container" style="padding: 10px;">
			<div class="container">
				<div class="center">
					<form on:submit={submitForm} class="form-container">
						<h2>データセット</h2>
						<input type="file" name="file" accept=".txt">
						<br>
						<button type="submit">保存</button>
					</form>
					<!-- メッセージ表示領域を追加 -->
					<p id="message"></p>
				</div>
			</div>		
		
			<div class="input-container">
				<input type="text" bind:value={genValue} placeholder="世代交代数">
				<button on:click={compileAndRunCpp}> 探索 </button>
			</div>
		</div>	
		{#if dataFromAPI.plotdata.length > 0}
			<div class="scatter" style="padding: 20px;">
				<Scatter data={chartData} {options} />
			</div>
		{:else}
			<p>データがありません</p>
		{/if}
		<div class="table1" style="display: flex; justify-content: center; padding: 10px;">
			<table>
				<tr>
					<th>　誤り率　</th>
					<th>　ノード数　</th>
				</tr>
				<tr>
					<td>{errorRate} %</td>
					<td>{nodeCount}</td>
				</tr>
			</table>
		</div>
	</div>
</div>

{#if imageBase64}
  <div class="img_container">
	  <img src={`data:image/png;base64,${imageBase64}`} alt="Visual representation of a tree structure" />
  </div>
{/if}
</div>
